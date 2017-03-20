// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/person_tracker_adapter.h"

#include <nan.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <v8.h>

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "common/camera-options/camera_options_host_instance.h"
#include "common/camera-options/camera_options_coprocessor.h"
#include "common/geometry/rect2d.h"
#include "person_info.h"
#include "person_tracker.h"
#include "skeleton_point_info.h"
#include "worker/person_data_internal.h"

#define PERSON_TRACKING_DATA_FILES "/usr/share/librealsense/pt/data/"

PersonTrackerAdapter* PersonTrackerAdapter::tracker_instance_ = nullptr;
const PersonTrackerAdapter::PesonTrackerStateMap PersonTrackerAdapter::state_maps_[kStateCount] = { //NOLINT
  { kStateReady, "ready" },
  { kStateRunning, "running" },
  { kStateDetecting, "detecting" },
  { kStateTracking, "tracking" },
  { kStatePaused, "paused" },
  { kStateError, "errored" }
};

const char* PersonTrackerAdapter::event_names_[2] = {
  "persontracked",
  "frameprocessed"
};

PersonTrackerAdapter::PersonTrackerAdapter()
    : device_started_(false),
      device_(nullptr),
      frame_data_for_user_(nullptr) {
  ResetInternalData();
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  host->GetCameraOptionsIO()->Add(this);
}

PersonTrackerAdapter::~PersonTrackerAdapter() {
  actual_module_config_.projection->release();
  js_this_.Reset();
}

FrameData* PersonTrackerAdapter::GetFrameData() {
  FrameData* ptr = frame_data_for_user_;
  frame_data_for_user_ = nullptr;
  return ptr;
}

// check wether the module support this option
bool PersonTrackerAdapter::IsSupported(const CameraOptionsType* src) const {
  const CameraOptionsType& options = *src;
  if (!options.has_member_color || !options.has_member_depth)
    return false;
  const auto& color_stream = options.member_color;
  const auto& depth_stream = options.member_depth;
  if (!color_stream.has_member_width ||
      !color_stream.has_member_height ||
      !color_stream.has_member_frameRate ||
      !color_stream.has_member_isEnabled ||
      !depth_stream.has_member_width ||
      !depth_stream.has_member_height ||
      !depth_stream.has_member_frameRate ||
      !depth_stream.has_member_isEnabled) {
    return false;
  }
  if (!color_stream.member_isEnabled ||
      !depth_stream.member_isEnabled)
    return false;

  rs::core::video_module_interface::supported_module_config config;
  for (int i = 0; ; ++i) {
    rs::core::status st = pt_module_->query_supported_module_config(i, config);
    if (st != rs::core::status_no_error)
      return false;
    auto& color = config[rs::core::stream_type::color];
    auto& depth = config[rs::core::stream_type::depth];
    auto& expected = options;
    if (color.size.width == expected.member_color.member_width &&
        color.size.height == expected.member_color.member_height &&
        color.frame_rate == expected.member_color.member_frameRate &&
        depth.size.width == expected.member_depth.member_width &&
        depth.size.height == expected.member_depth.member_height &&
        depth.frame_rate == expected.member_depth.member_frameRate)
      return true;
  }
  return false;
}

std::string PersonTrackerAdapter::GetUnsupportedMessage() const {
  // TODO(shaoting): find the actually supported options.
  const char* full_msg =
  "You've set CameraOptions to Person Tracking that are not currently supported." "\n" // NOLINT
  "Try set one of the options in below list:" "\n"
  "-----------------------------------------" "\n"
  "color: 320x240,   depth: 320x240, fps: 30" "\n"
  "color: 320x240,   depth: 628x468, fps: 30" "\n"
  "color: 640x480,   depth: 320x240, fps: 30" "\n"
  "color: 640x480,   depth: 480x360, fps: 30" "\n"
  "color: 640x480,   depth: 628x468, fps: 30" "\n"
  "color: 1280x720,  depth: 628x468, fps: 30" "\n"
  "color: 1920x1080, depth: 628x468, fps: 30" "\n"
  "-----------------------------------------" "\n";

  return full_msg;
}

void PersonTrackerAdapter::ApplyToCamera(const CameraOptionsType* src) {
  config_.camera_options_ = std::make_shared<DictionaryCameraOptions>(*src);
}

void PersonTrackerAdapter::PartiallyFillInCameraData(
    CameraOptionsType* target) {
  // TODO(shaoting) implement when used.
}

bool PersonTrackerAdapter::SetConfig(
    const PersonTrackerOptions& cfg, std::string* error) {
  return SetConfig(
      std::make_shared<DictionaryPersonTrackerOptions>(cfg), error);
}

bool PersonTrackerAdapter::SetConfig(
    std::shared_ptr<DictionaryPersonTrackerOptions> cfg, std::string* error) {
  return config_.option_helper_.SetOptions(cfg, error);
}

bool PersonTrackerAdapter::GetConfig(PersonTrackerOptions* options) {
  return config_.GetConfigInfo(options);
}

PersonTrackerAdapter* PersonTrackerAdapter::Instance() {
  if (!tracker_instance_)
    tracker_instance_ = new PersonTrackerAdapter();
  return tracker_instance_;
}

void PersonTrackerAdapter::DestroyInstance() {
  if (tracker_instance_)
    delete tracker_instance_;
  tracker_instance_ = nullptr;
}

v8::Local<v8::Promise> PersonTrackerAdapter::CreateRejectedPromise(
    std::string reason) {
  PromiseHelper helper;
  v8::Local<v8::Promise> promise = helper.CreatePromise();
  helper.RejectPromise(reason);
  return promise;
}

v8::Local<v8::Object> PersonTrackerAdapter::CreateResultObject() {
  // create nan wrapper object
  PersonTrackingResult* result_impl = internal_result_.ToPersonTrackResult();
  auto result = NanPersonTrackingResult::NewInstance(result_impl);
  return result;
}

void PersonTrackerAdapter::BeforeEmitResult() {
  bool update_internal_state = false;
  bool update_frame = true;

  if (state_ == kStatePaused || state_ == kStateError)
    update_frame = false;

  // only expose internal state when the mw is running.
  if (IsRunning() && has_internal_state_update_)
    update_internal_state = true;

  // The internal state is from mw, and store it to the state_
  // to let user known the state.
  if (update_internal_state) {
    SetState(internal_state_);
    has_internal_state_update_ = false;
  }

  // prepare the frame data to be retrived by main thread user.
  if (update_frame) {
    if (frame_data_for_user_)
      delete frame_data_for_user_;
    frame_data_for_user_ = internal_result_.GrabFrameData();
  }
}

void PersonTrackerAdapter::AfterEmitResult() {
  // the data has been Consumed, empty it.
  CleanResultObject(false);
}

size_t PersonTrackerAdapter::GetEventCount() {
  return 2;
}

bool PersonTrackerAdapter::ShouldPopEvent(size_t event_index) {
  if (IsRunning()) {
    const char* api_name_string = "listenerCount";
    v8::Local<v8::String> api_name = Nan::New(api_name_string).ToLocalChecked();
    v8::Local<v8::String> event_name =
        Nan::New(event_names_[event_index]).ToLocalChecked();
    v8::Local<v8::Object> js_this = Nan::New(js_this_);
    Nan::Maybe<bool> has_api_maybe = Nan::Has(js_this, api_name);
    bool has_api = has_api_maybe.FromMaybe(false);
    if (has_api) {
      v8::Local<v8::Value> args[1] =
          {Nan::New(event_names_[event_index]).ToLocalChecked()};
      v8::Local<v8::Value> result =
          Nan::MakeCallback(js_this, api_name_string, 1, args);
      int32_t count = result->Int32Value();
      if (count) {
        if (event_index == 0 && !internal_result_.persons_.size())
          return false;
        return true;
      }
    }
  }
  return false;
}

v8::Local<v8::Value> PersonTrackerAdapter::PopEventData(size_t event_index) {
  return CreateResultObject();
}

std::string PersonTrackerAdapter::GetEventName(size_t event_index) {
  if (event_index < 2)
    return event_names_[event_index];
  return "";
}

void PersonTrackerAdapter::CleanResultObject(bool clean_all) {
  internal_result_.Reset();
  if (clean_all && frame_data_for_user_) {
    delete frame_data_for_user_;
    frame_data_for_user_ = nullptr;
  }
}

void PersonTrackerAdapter::ConfigureCameraUsingSpecifiedData() {
  auto& color = config_.camera_options_->member_color;
  auto& depth = config_.camera_options_->member_depth;
  DEBUG_INFO("Enabling color stream:", "\t", color.member_width, "x",
      color.member_height, "x", color.member_frameRate, "format rgb8");
  device_->enable_stream(
      rs::stream::color,
      color.member_width,
      color.member_height,
      rs::format::rgb8,
      color.member_frameRate);
  rs::core::video_module_interface::actual_image_stream_config&
      actual_stream_config_color = actual_module_config_[rs::core::stream_type::color];  // NOLINT
  actual_stream_config_color.size.width = color.member_width;
  actual_stream_config_color.size.height = color.member_height;
  actual_stream_config_color.frame_rate = color.member_frameRate;
  actual_stream_config_color.intrinsics = rs::utils::convert_intrinsics(
      device_->get_stream_intrinsics(rs::stream::color));
  actual_stream_config_color.extrinsics = rs::utils::convert_extrinsics(
      device_->get_extrinsics(rs::stream::depth, rs::stream::color));
  actual_stream_config_color.is_enabled = true;
  DEBUG_INFO("Enabling depth stream:", "\t", depth.member_width, "x",
      depth.member_height, "x", depth.member_frameRate, "format z16");
  device_->enable_stream(
      rs::stream::depth,
      depth.member_width,
      depth.member_height,
      rs::format::z16,
      depth.member_frameRate);

  CameraOptionsCoProcessor cop(*config_.camera_options_,
    CameraOptionsCoProcessor::PersonTracking);
  cop.TryEnableExtraChannels(device_);
  cop.TrySetExtraOptions(device_);

  rs::core::video_module_interface::actual_image_stream_config&
      actual_stream_config_depth = actual_module_config_[rs::core::stream_type::depth];  // NOLINT
  actual_stream_config_depth.size.width = depth.member_width;
  actual_stream_config_depth.size.height = depth.member_height;
  actual_stream_config_depth.frame_rate = depth.member_frameRate;
  actual_stream_config_depth.intrinsics = rs::utils::convert_intrinsics(
      device_->get_stream_intrinsics(rs::stream::depth));
  actual_stream_config_depth.extrinsics = rs::utils::convert_extrinsics(
      device_->get_extrinsics(rs::stream::depth, rs::stream::depth));
  actual_stream_config_depth.is_enabled = true;
}

bool PersonTrackerAdapter::ConfigureCamera() {
  actual_module_config_ = {};

  ctx_ = CameraDelegateInstance::GetInstance();

  // Check if there is request to use playback file as virtual camera
  if (config_.camera_options_->has_member_playbackPathForTesting) {
    const char* playback_filename =
        config_.camera_options_->member_playbackPathForTesting.c_str();
    std::ifstream playback_file(playback_filename);
    if (playback_file.good())
      ctx_->ConnectToVirtualCamera(playback_filename);
  }

  if (ctx_->get_device_count() == 0) {
    DEBUG_ERROR("cant find devices");
    return false;
  }

  if (!device_)
    device_ = ctx_->get_device(0);

  ConfigureCameraUsingSpecifiedData();

  // enable auto exposure for color stream
  device_->set_option(rs::option::color_enable_auto_exposure, 1);

  // enable auto exposure for Depth camera stream
  device_->set_option(rs::option::r200_lr_auto_exposure_enabled, 1);

  rs::core::intrinsics color_intrin = rs::utils::convert_intrinsics(
      device_->get_stream_intrinsics(rs::stream::color));
  rs::core::intrinsics depth_intrin = rs::utils::convert_intrinsics(
      device_->get_stream_intrinsics(rs::stream::depth));
  rs::core::extrinsics extrinsics = rs::utils::convert_extrinsics(
     device_->get_extrinsics(rs::stream::depth, rs::stream::color));
  actual_module_config_.projection =
      rs::core::projection_interface::create_instance(
          &color_intrin, &depth_intrin, &extrinsics);
  camera_configured_ = true;
  return true;
}

bool PersonTrackerAdapter::Start() {
  if (!camera_configured_) {
    if (!ConfigureCamera()) {
      SetErrorDescription("Failed to configure camera");
      SetState(kStateError);
      return false;
    }
  }

  if (!module_configured_) {
    if (!ConfigureModule()) {
      SetErrorDescription("Failed to configure module");
      SetState(kStateError);
      return false;
    }
  }

  // set the enabled module configuration
  if (!video_module_configured_) {
    if (pt_module_->set_module_config(actual_module_config_) !=
        rs::core::status_no_error) {
      SetErrorDescription("Failed to set the module configuration");
      return false;
    }
    video_module_configured_ = true;
  }

  if (!device_started_) {
    try {
      CameraOptionsCoProcessor cop(*config_.camera_options_,
        CameraOptionsCoProcessor::ObjectRecognition);
      device_->start(cop.GetCameraStartOptions(device_));
      SetState(kStateRunning);
    } catch (std::exception& e) {
      DEBUG_ERROR("start device failed,", e.what());
      SetErrorDescription("Failed to start camera");
      SetState(kStateError);
      return false;
    }
    device_started_ = true;
  }
  return true;
}

void PersonTrackerAdapter::ResetTracking() {
  // Reset the PT module.
  auto pdata = pt_module_->QueryOutput();
  if (pdata) {
    pdata->ResetTracking();
  }
}

void PersonTrackerAdapter::Stop() {
  StopCamera();
  ResetTracking();
  ResetStateData();
}

void PersonTrackerAdapter::Pause() {
  SetState(kStatePaused);
}

void PersonTrackerAdapter::Resume() {
  SetState(kStateRunning);
}

void PersonTrackerAdapter::SetJavaScriptThis(v8::Local<v8::Object> obj) {
  js_this_.Reset(obj);
}

v8::Local<v8::Object> PersonTrackerAdapter::GetJavaScriptThis() {
  return Nan::New(js_this_);
}

void PersonTrackerAdapter::ResetJSObject() {
  const char* api_name_string = "removeAllListeners";
  v8::Local<v8::String> api_name = Nan::New(api_name_string).ToLocalChecked();
  v8::Local<v8::Object> js_this = Nan::New(js_this_);
  Nan::Maybe<bool> has_api_maybe = Nan::Has(js_this, api_name);
  bool has_api = has_api_maybe.FromMaybe(false);
  if (has_api) {
    for (int32_t i=0; i < sizeof(event_names_)/sizeof(const char*); i++) {
      v8::Local<v8::Value> args[1] =
          {Nan::New(event_names_[i]).ToLocalChecked()};
      Nan::MakeCallback(js_this, api_name_string, 1, args);
    }
  } else {
    DEBUG_ERROR("The tracker instance doesn't have removeAllListeners")
  }
}

void PersonTrackerAdapter::Reset() {
  StopCamera();
  ResetInternalData();
}

void PersonTrackerAdapter::StopCamera() {
  try {
    if (device_ && device_started_)
      device_->stop();
    device_started_ = false;
    delete device_;
    device_ = nullptr;
    camera_configured_ = false;
  } catch (std::exception& e) {
    DEBUG_ERROR("stop device error: ", e.what());
  }
}

void PersonTrackerAdapter::ResetStateData() {
  SetState(kStateReady);
  internal_state_ = kStateReady;
  has_internal_state_update_ = false;
}

void PersonTrackerAdapter::ResetInternalData() {
  config_.Reset();
  ctx_ = nullptr;  // No need to delete singleton object
  pt_module_.reset(
      rs::person_tracking::person_tracking_video_module_factory::create_person_tracking_video_module(GetDataFilesPath().c_str())); //NOLINT
  actual_module_config_ = {};
  camera_configured_ = false;
  module_configured_ = false;
  video_module_configured_ = false;
  sample_set_.reset();
  frame_data_backup_.reset();
  ResetStateData();
}

using ExpressionType = PTDATA::PersonExpressions;
using TrackingConfig = PT::PersonTrackingConfiguration::TrackingConfiguration;
using SkeletonConfig = PT::SkeletonJointsConfiguration;

void PersonTrackerAdapter::QueryModuleSupportedConfigs() {
  for (int i = 0; ; i++) {
    auto config =
        std::make_shared<rs::core::video_module_interface::supported_module_config>();  // NOLINT
    rs::core::status st = pt_module_->query_supported_module_config(i, *config);
    if (st != rs::core::status_no_error) {
      break;
    }
    auto& color = (*config)[rs::core::stream_type::color];
    auto& depth = (*config)[rs::core::stream_type::depth];
    // skip those with 0 frame rate
    if (color.frame_rate && depth.frame_rate) {
      module_supported_configs_.push_back(config);
      DEBUG_INFO("Module Config ", i, ": color: ", color.size.width, "x",
          color.size.height, " ", color.frame_rate, "fps", " depth: ",
          depth.size.width, "x", depth.size.height, " ", depth.frame_rate,
          "fps");
    }
  }
}

bool PersonTrackerAdapter::ConfigureModule() {
  // Query the module supported configs
  QueryModuleSupportedConfigs();

  // return if there is no config data.
  if (config_.option_helper_.options_) {
    PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();

    // track config
    auto tkcfg = pt_configuration->QueryTracking();
    if (config_.option_helper_.IsTrackingEnabled()) {
      tkcfg->Enable();

      const DictionaryTrackingOptions& tkoptions =
          config_.option_helper_.options_->member_tracking;

      if (tkoptions.has_member_maxTrackedPerson)
        tkcfg->SetMaxTrackedPersons(tkoptions.member_maxTrackedPerson);

      if (tkoptions.has_member_trackingMode) {
        if (!tkoptions.member_trackingMode.compare("following"))
          tkcfg->SetTrackingMode(TrackingConfig::TRACKING_MODE_FOLLOWING);
        else if (!tkoptions.member_trackingMode.compare("interactive"))
          tkcfg->SetTrackingMode(TrackingConfig::TRACKING_MODE_INTERACTIVE);
        else if (!tkoptions.member_trackingMode.compare("single-person"))
          tkcfg->SetTrackingMode(TrackingConfig::TRACKING_MODE_SINGLE_PERSON);
        else
          return false;
      }

      if (tkoptions.has_member_detectionMode) {
        if (!tkoptions.member_detectionMode.compare("auto"))
          tkcfg->SetDetectionMode(TrackingConfig::AUTO);
        else if (!tkoptions.member_detectionMode.compare("close-range"))
          tkcfg->SetDetectionMode(TrackingConfig::MANUAL_CLOSE_RANGE);
        else if (!tkoptions.member_detectionMode.compare("mid-range"))
          tkcfg->SetDetectionMode(TrackingConfig::MANUAL_MID_RANGE);
        else if (!tkoptions.member_detectionMode.compare("far-range"))
          tkcfg->SetDetectionMode(TrackingConfig::MANUAL_FAR_RANGE);
        else if (!tkoptions.member_detectionMode.compare("full-range"))
          tkcfg->SetDetectionMode(TrackingConfig::MANUAL_ALL);
        else
          return false;
      }

      if (tkoptions.has_member_enableSegmentation) {
        if (tkoptions.member_enableSegmentation)
          tkcfg->EnableSegmentation();
        else
          tkcfg->DisableSegmentation();
      }

      if (tkoptions.has_member_enableBlob) {
        if (tkoptions.member_enableBlob)
          tkcfg->EnableBlob();
        else
          tkcfg->DisableBlob();
      }
#ifdef PT_MW_DEV
      if (tkoptions.has_member_enablePersonOrientation) {
        if (tkoptions.member_enablePersonOrientation)
          tkcfg->EnablePersonOrientation();
        else
          tkcfg->DisablePersonOrientation();
      }
#endif
      if (tkoptions.has_member_enableHeadBoundingBox) {
        if (tkoptions.member_enableHeadBoundingBox)
          tkcfg->EnableHeadBoundingBox();
        else
          tkcfg->DisableHeadBoundingBox();
      }

      if (tkoptions.has_member_enableDetectionFromFar) {
        if (tkoptions.member_enableDetectionFromFar)
          tkcfg->EnableDetectionFromFar();
        else
          tkcfg->DisableDetectionFromFar();
      }
    } else if (config_.option_helper_.IsTrackingDisabled()) {
      tkcfg->Disable();
    }

    // skeleton config
    auto skcfg = pt_configuration->QuerySkeletonJoints();
    if (config_.option_helper_.IsSkeletonEnabled()) {
      const DictionarySkeletonRecognitionOptions& skoptions =
          config_.option_helper_.options_->member_skeleton;
      skcfg->Enable();

      if (skoptions.has_member_maxTrackedPerson)
        skcfg->SetMaxTrackedPersons(skoptions.member_maxTrackedPerson);

      if (skoptions.has_member_trackingArea) {
        if (!skoptions.member_trackingArea.compare("upper-body")) {
          skcfg->SetTrackingArea(SkeletonConfig::AREA_UPPER_BODY);
        } else if (!skoptions.member_trackingArea.compare("upper-body-rough")) {
          skcfg->SetTrackingArea(SkeletonConfig::AREA_UPPER_BODY_ROUGH);
        } else if (!skoptions.member_trackingArea.compare("full-body-rough")) {
          skcfg->SetTrackingArea(SkeletonConfig::AREA_FULL_BODY_ROUGH);
        } else if (!skoptions.member_trackingArea.compare("full-body")) {
          skcfg->SetTrackingArea(SkeletonConfig::AREA_FULL_BODY);
        } else {
          return false;
        }
      }
    } else if (config_.option_helper_.IsSkeletonDisabled()) {
      skcfg->Disable();
    }

    // gesture config
    auto gescfg = pt_configuration->QueryGestures();
    if (config_.option_helper_.IsGestureEnabled()) {
      const DictionaryGestureRecognitionOptions& gesoptions
          = config_.option_helper_.options_->member_gesture;
      gescfg->Enable();
      if (gesoptions.has_member_maxTrackedPerson)
        gescfg->SetMaxTrackedPersons(gesoptions.member_maxTrackedPerson);

      if (gesoptions.has_member_enableAllGestures) {
        if (gesoptions.member_enableAllGestures) {
          gescfg->EnableAllGestures();
        }
      }

      if (gesoptions.has_member_disableAllGestures) {
        if (gesoptions.member_disableAllGestures) {
          gescfg->DisableAllGestures();
        }
      }

      if (gesoptions.has_member_enablePointing) {
        if (gesoptions.member_enablePointing) {
          gescfg->EnableGesture(PTDATA::PersonGestures::Pointing);
        } else {
          gescfg->DisableGesture(PTDATA::PersonGestures::Pointing);
        }
      }
    } else if (config_.option_helper_.IsGestureDisabled()) {
      gescfg->Disable();
    }

    // person face
    auto facecfg = pt_configuration->QueryFace();
    if (config_.option_helper_.options_->has_member_personFace) {
      const DictionaryPersonFaceOptions& pfoptions =
          config_.option_helper_.options_->member_personFace;

      if (config_.option_helper_.IsHeadPoseEnabled())
        facecfg->EnableHeadPose();
      else if (config_.option_helper_.IsHeadPoseDisabled())
        facecfg->DisableHeadPose();

      if (config_.option_helper_.IsFaceLandmarkEnabled())
        facecfg->EnableFaceLandmarks();
      else if (config_.option_helper_.IsFaceLandmarkDisabled())
        facecfg->DisableFaceLandmarks();
#ifdef PT_MW_DEV
      auto expcfg = facecfg->QueryExpressions();
      if (config_.option_helper_.IsExpressionEnabled()) {
        const DictionaryExpressionRecognitionOptions& expoptions =
            pfoptions.member_expression;
        expcfg->Enable();
        if (expoptions.has_member_maxTrackedPerson)
          expcfg->SetMaxTrackedPeople(expoptions.member_maxTrackedPerson);

        if (expoptions.has_member_enableAllExpressions
            && expoptions.member_enableAllExpressions)
          expcfg->EnableAllExpressions();

        if (expoptions.has_member_disableAllExpressions
            && expoptions.member_disableAllExpressions)
          expcfg->DisableAllExpressions();

        if (expoptions.has_member_enableNeutral) {
          if (expoptions.member_enableNeutral)
            expcfg->EnableExpression(ExpressionType::NEUTRAL);
          else
            expcfg->DisableExpression(ExpressionType::NEUTRAL);
        }

        if (expoptions.has_member_enableHappy) {
          if (expoptions.member_enableHappy)
            expcfg->EnableExpression(ExpressionType::HAPPINESS);
          else
            expcfg->DisableExpression(ExpressionType::HAPPINESS);
        }

        if (expoptions.has_member_enableSad) {
          if (expoptions.member_enableSad)
            expcfg->EnableExpression(ExpressionType::SADNESS);
          else
            expcfg->DisableExpression(ExpressionType::SADNESS);
        }

        if (expoptions.has_member_enableSurprise) {
          if (expoptions.member_enableSurprise)
            expcfg->EnableExpression(ExpressionType::SURPRISE);
          else
            expcfg->DisableExpression(ExpressionType::SURPRISE);
        }

        if (expoptions.has_member_enableFear) {
          if (expoptions.member_enableFear)
            expcfg->EnableExpression(ExpressionType::FEAR);
          else
            expcfg->DisableExpression(ExpressionType::FEAR);
        }

        if (expoptions.has_member_enableAnger) {
          if (expoptions.member_enableAnger)
            expcfg->EnableExpression(ExpressionType::ANGER);
          else
            expcfg->DisableExpression(ExpressionType::ANGER);
        }

        if (expoptions.has_member_enableDisgust) {
          if (expoptions.member_enableDisgust)
            expcfg->EnableExpression(ExpressionType::DISGUST);
          else
            expcfg->DisableExpression(ExpressionType::DISGUST);
        }

        if (expoptions.has_member_enableContempt) {
          if (expoptions.member_enableContempt)
            expcfg->EnableExpression(ExpressionType::CONTEMPT);
          else
            expcfg->DisableExpression(ExpressionType::CONTEMPT);
        }
      } else if (config_.option_helper_.IsExpressionDisabled()) {
        expcfg->Disable();
      }
#endif  // PT_MW_DEV
    }

    // recognition
    if (config_.option_helper_.options_->has_member_recognition) {
      auto recogcfg = pt_configuration->QueryRecognition();
      const DictionaryPersonRecognitionOptions& recognitions =
          config_.option_helper_.options_->member_recognition;

      if (recognitions.has_member_enable && recognitions.member_enable
          || !recognitions.has_member_enable) {
        recogcfg->Enable();

        if (recognitions.has_member_policy) {
          if (!recognitions.member_policy.compare("standard"))
            recogcfg->SetPolicy(PT::RecognitionConfiguration::Standard);
        }
      } else {
        recogcfg->Disable();
      }
    }
#ifdef PT_MW_DEV
    if (config_.option_helper_.options_->has_member_pose) {
      auto posecfg = pt_configuration->QueryPose();
      const DictionaryPersonPoseRecognitionOptions& poseoptions =
          config_.option_helper_.options_->member_pose;

      if (poseoptions.has_member_enable && poseoptions.member_enable
          || !poseoptions.has_member_enable) {
        posecfg->Enable();

        if (poseoptions.has_member_maxTrackedPerson)
          posecfg->SetMaxTrackedPersons(poseoptions.member_maxTrackedPerson);
      } else {
        posecfg->Disable();
      }
    }

    if (config_.option_helper_.options_->has_member_lying) {
      auto lyingcfg = pt_configuration->QueryPersonLying();
      const DictionaryLyingPoseRecognitionOptions& lyoptions =
          config_.option_helper_.options_->member_lying;

      if (lyoptions.has_member_enable && lyoptions.member_enable
          || !lyoptions.has_member_enable) {
        lyingcfg->Enable();

        if (lyoptions.has_member_maxTrackedPerson)
          lyingcfg->SetMaxTrackedPersons(lyoptions.member_maxTrackedPerson);
        // TODO(shaoting): detection state set here or through
        // Instance.PersonLyingControl?
      } else {
        lyingcfg->Disable();
      }
    }
#endif
  }
  module_configured_ = true;
  return true;
}

void PersonTrackerAdapter::BackupFrameDataInWorkerThread(
    rs::stream* stream,
    int* height,
    int* width,
    rs::format* format,
    double* timestamp,
    uint64_t* framenumber,
    const void** framedata) {
  frame_data_backup_.reset(new FrameDataAssembler);
  for (int i=0; i < 2; i++) {
    frame_data_backup_->SetData(stream[i], width[i], height[i], format[i],
        timestamp[i], framenumber[i], static_cast<const char*>(framedata[i]));
  }
}

void PersonTrackerAdapter::ShareFrameDataToMainThread() {
  internal_result_.frame_data_ = frame_data_backup_;
  frame_data_backup_.reset();
}

int32_t PersonTrackerAdapter::GetNextFrame() { //NOLINT
  try {
    device_->wait_for_frames();
  } catch (std::exception& e) {
    DEBUG_ERROR("Exception when waiting for frames:", e.what());
    return -1;
  }
  // TODO(shaoting): Is the image memory owned by mw?
  auto sample_set = new rs::core::correlated_sample_set();
  memset(sample_set, 0, sizeof(sample_set_));
  sample_set_.reset(sample_set);
  rs::stream librealsense_stream[2] = {rs::stream::color};
  int height[2] = {0}, width[2] = {0};
  double timestamp[2] = {0};
  uint64_t framenumber[2] = {0};
  const void* framedata[2] = {0};
  rs::format format[2] = {rs::format::any};
  int index = 0;

  for (auto& stream : { rs::core::stream_type::color,
                        rs::core::stream_type::depth }) {
    librealsense_stream[index] = rs::utils::convert_stream_type(stream);
    height[index] = device_->get_stream_height(librealsense_stream[index]);
    width[index] = device_->get_stream_width(librealsense_stream[index]);
    timestamp[index] = device_->get_frame_timestamp(
        librealsense_stream[index]);
    framenumber[index] = device_->get_frame_number(
        librealsense_stream[index]);
    framedata[index] = device_->get_frame_data(librealsense_stream[index]);
    format[index] = device_->get_stream_format(librealsense_stream[index]);

    if (!framedata[index]) {
      DEBUG_ERROR("frame data for ", librealsense_stream, " is null");
      return -1;
    }

    rs::core::image_info info = {
      width[index],
      height[index],
      rs::utils::convert_pixel_format(format[index]),
      image_utils::GetPixelSize(format[index])* width[index]
    };

    rs::core::image_interface* image =
        rs::core::image_interface::create_instance_from_raw_data(
            &info,
            rs::core::image_interface::image_data_with_data_releaser(
                framedata[index], nullptr),
            stream,
            rs::core::image_interface::flag::any,
            timestamp[index],
            framenumber[index]);
    (*sample_set_)[stream] = image;
    index++;
  }
  BackupFrameDataInWorkerThread(librealsense_stream, height, width, format,
      timestamp, framenumber, framedata);
  return 0;
}

bool PersonTrackerAdapter::IterationWork() {
  if (device_->is_streaming()) {
    // get frame
    if (GetNextFrame() != 0) {
      DEBUG_ERROR("invalid_frame");
      return false;
    }

    // release those images for the previous samples.
    ReleaseSegmentationImages();

    if (pt_module_->process_sample_set(*sample_set_) !=
        rs::core::status_no_error) {
      DEBUG_ERROR("failed to process sample");
      return false;
    }
    ProcessOutput(pt_module_->QueryOutput());
    return true;
  }

  return false;
}

std::wstring PersonTrackerAdapter::GetDataFilesPath() {
  struct stat stat_struct;

  if (stat(PERSON_TRACKING_DATA_FILES, &stat_struct) != 0) {
    DEBUG_ERROR("Failed to find person tracking data files at ",
        PERSON_TRACKING_DATA_FILES);
    DEBUG_ERROR("Please check that you run sample from correct directory");
    exit(EXIT_FAILURE);
  }

  std::string person_tracking_data_files = PERSON_TRACKING_DATA_FILES;
  int size = person_tracking_data_files.length();
  wchar_t wc_person_tracking_data_files[size + 1] = {0};
  mbstowcs(wc_person_tracking_data_files,
     person_tracking_data_files.c_str(),
     size + 1);
  return std::wstring(wc_person_tracking_data_files);
}

bool PersonTrackerAdapter::IsStreamCompatible(
    rs::core::stream_type stream,
    rs::format format,
    int width,
    int height,
    int frame_rate) {
  bool frame_rate_ok = (frame_rate == 30);
  bool color_format_ok = false;
  bool resolution_ok = false;

  switch (stream) {
    case rs::core::stream_type::color:
      color_format_ok = (format == rs::format::rgb8);
      resolution_ok = (width == PersonTrackingConfig::GetResolutionWidth(
          config_.color_resolution_)) &&
          (height == PersonTrackingConfig::GetResolutionHeight(
              config_.color_resolution_));
      break;

    case rs::core::stream_type::depth:
      color_format_ok =  (format == rs::format::z16);
      resolution_ok = (width == PersonTrackingConfig::GetResolutionWidth(config_.depth_resolution_)) && // NOLINT
          (height == PersonTrackingConfig::GetResolutionHeight(config_.depth_resolution_)); // NOLINT
      break;

    default:
      return  false;
  }

  return  color_format_ok && resolution_ok && frame_rate_ok;
}

void PersonTrackerAdapter::ExtractLandmarkData(PTDATA::Person* p,
    PersonDataInternal* result) {
  auto face = p->QueryFace();

  if (face) {
    int32_t num = face->QueryNumLandmarks();

    if (num) {
      PTDATA::PersonFace::LandmarksInfo* info =
          face->QueryLandmarks();

      if (info) {
        result->landmark_confidence_ = info->confidence;

        for (int32_t i = 0; i < num; i++) {
          FaceLandmark* mark = new FaceLandmark;

          if (mark) {
            mark->imageCoordinate_.set_x(info->landmarks[i].image.x);
            mark->imageCoordinate_.set_y(info->landmarks[i].image.y);
            mark->imageCoordinate_.set_z(info->landmarks[i].image.z);
            mark->worldCoordinate_.set_x(info->landmarks[i].world.x);
            mark->worldCoordinate_.set_y(info->landmarks[i].world.y);
            mark->worldCoordinate_.set_z(info->landmarks[i].world.z);
            result->landmarks_.push_back(mark);
          }
        }
      }
    }
  }
}

void PersonTrackerAdapter::ExtractPoseData(PTDATA::Person* p,
    PersonDataInternal* result) {
#ifdef PT_MW_DEV
  auto pose = p->QueryPose();

  if (pose) {
    auto info = pose->QueryPositionState();
    auto data = new PoseInfo;
    data->confidence_ = info.confidence;

    switch (info.position) {
      case PTDATA::PersonPose::POSITION_LYING_DOWN:
        data->position_ = "lying-down";
        break;

      case PTDATA::PersonPose::POSITION_SITTING:
        data->position_ = "sitting";
        break;

      case PTDATA::PersonPose::POSITION_STANDING:
        data->position_ = "standing";
        break;
    }

    result->pose_data_.reset(data);
  }
#endif
}

using JointsType = PTDATA::PersonJoints;

void PersonTrackerAdapter::ExtractSkeletonData(PTDATA::Person* p,
    PersonDataInternal* result) {
  JointsType* joints = p->QuerySkeletonJoints();

  if (joints) {
    int32_t num = joints->QueryNumJoints();

    if (num) {
      auto pts = new JointsType::SkeletonPoint[num];

      if (pts && joints->QueryJoints(pts)) {
        for (int32_t i = 0; i < num; i++) {
          SkeletonPointInfo* info = new SkeletonPointInfo();

          if (info) {
            info->worldCoordinate_.set_x(pts[i].world.x);
            info->worldCoordinate_.set_y(pts[i].world.y);
            info->worldCoordinate_.set_z(pts[i].world.z);
            info->worldConfidence_ = pts[i].confidenceWorld;
            info->imageCoordinate_.set_x(pts[i].image.x);
            info->imageCoordinate_.set_y(pts[i].image.y);
            info->imageConfidence_ = pts[i].confidenceImage;
            switch (pts[i].jointType) {
              case JointsType::JOINT_ANKLE_LEFT:
                info->jointType_ = "ankle-left";
                break;

              case JointsType::JOINT_ANKLE_RIGHT:
                info->jointType_ = "ankle-right";
                break;

              case JointsType::JOINT_ELBOW_LEFT:
                info->jointType_ = "elbow-left";
                break;

              case JointsType::JOINT_ELBOW_RIGHT:
                info->jointType_ = "elbow-right";
                break;

              case JointsType::JOINT_FOOT_LEFT:
                info->jointType_ = "foot-left";
                break;

              case JointsType::JOINT_FOOT_RIGHT:
                info->jointType_ = "foot-right";
                break;

              case JointsType::JOINT_HAND_LEFT:
                info->jointType_ = "hand-left";
                break;

              case JointsType::JOINT_HAND_RIGHT:
                info->jointType_ = "hand-right";
                break;

              case JointsType::JOINT_HAND_TIP_LEFT:
                info->jointType_ = "hand-tip-left";
                break;

              case JointsType::JOINT_HAND_TIP_RIGHT:
                info->jointType_ = "hand-tip-right";
                break;

              case JointsType::JOINT_HEAD:
                info->jointType_ = "head";
                break;

              case JointsType::JOINT_HIP_LEFT:
                info->jointType_ = "hip-left";
                break;

              case JointsType::JOINT_HIP_RIGHT:
                info->jointType_ = "hip-right";
                break;

              case JointsType::JOINT_KNEE_LEFT:
                info->jointType_ = "knee-left";
                break;

              case JointsType::JOINT_KNEE_RIGHT:
                info->jointType_ = "knee-right";
                break;

              case JointsType::JOINT_NECK:
                info->jointType_ = "neck";
                break;

              case JointsType::JOINT_SHOULDER_LEFT:
                info->jointType_ = "shoulder-left";
                break;

              case JointsType::JOINT_SHOULDER_RIGHT:
                info->jointType_ = "shoulder-right";
                break;

              case JointsType::JOINT_SPINE_BASE:
                info->jointType_ = "spine-base";
                break;

              case JointsType::JOINT_SPINE_MID:
                info->jointType_ = "spine-mid";
                break;

              case JointsType::JOINT_SPINE_SHOULDER:
                info->jointType_ = "spine-shoulder";
                break;

              case JointsType::JOINT_THUMB_LEFT:
                info->jointType_ = "thumb-left";
                break;

              case JointsType::JOINT_THUMB_RIGHT:
                info->jointType_ = "thumb-right";
                break;

              case JointsType::JOINT_WRIST_LEFT:
                info->jointType_ = "wrist-left";
                break;

              case JointsType::JOINT_WRIST_RIGHT:
                info->jointType_ = "wrist-right";
                break;

              default:
                break;
            }

            result->skeleton_joints_.push_back(info);
          }
        }
      }

      if (pts)
        delete [] pts;
    }
  }
}

bool PersonTrackerAdapter::BuildupMaskInfoFromImage(RS::Image* image, bool is3D,
    MaskInfo* output) {
  if (image) {
    RS::Image::ImageInfo seg_image_info = image->QueryInfo();
    RS::Image::ImageData seg_image_data;
    image->ExportData(&seg_image_data, 0);
    output->width_ = seg_image_info.width;
    output->height_ = seg_image_info.height;
    output->maskData_.size = output->width_ * output->height_;
    output->maskData_.data = reinterpret_cast<char*>(
        malloc(output->maskData_.size));
    memcpy(reinterpret_cast<void*>(output->maskData_.data),
        reinterpret_cast<void*>(seg_image_data.planes[0]),
        output->maskData_.size);
    return true;
  }
  return false;
}

using TrackingType = RS::PersonTracking::PersonTracking;

void PersonTrackerAdapter::ExtractTrackData(PTDATA::Person* p,
    PersonDataInternal* result) {
  auto tracking = p->QueryTracking();
  if (tracking) {
    auto data = new TrackingInfo;
    data->id_ = tracking->QueryId();
    auto box = tracking->Query2DBoundingBox();

    if (box.confidence) {
      data->boundingBox_.rect_.set_x(box.rect.x);
      data->boundingBox_.rect_.set_y(box.rect.y);
      data->boundingBox_.rect_.set_width(box.rect.w);
      data->boundingBox_.rect_.set_height(box.rect.h);
      data->boundingBox_.confidence_ = box.confidence;
      data->has_box_ = true;
    }

    if (config_.option_helper_.options_
        ->member_tracking.member_enableHeadBoundingBox) {
      auto headbox = tracking->QueryHeadBoundingBox();

      if (headbox.confidence) {
        data->headBoundingBox_.rect_.set_x(headbox.rect.x);
        data->headBoundingBox_.rect_.set_y(headbox.rect.y);
        data->headBoundingBox_.rect_.set_width(headbox.rect.w);
        data->headBoundingBox_.rect_.set_height(headbox.rect.h);
        data->headBoundingBox_.confidence_ = headbox.confidence;
        data->has_head_box_ = true;
      }
    }

    auto center = tracking->QueryCenterMass();
    if (center.image.confidence) {
      data->center_.imageCoordinate_.set_x(center.image.point.x);
      data->center_.imageCoordinate_.set_y(center.image.point.y);
      data->center_.imageConfidence_ = center.image.confidence;
      data->has_center_ = true;
    }

    if (center.world.confidence) {
      data->center_.worldCoordinate_.set_x(center.world.point.x);
      data->center_.worldCoordinate_.set_y(center.world.point.y);
      data->center_.worldCoordinate_.set_z(center.world.point.z);
      data->center_.worldConfidence_ = center.world.confidence;
      data->has_center_ = true;
    }

    if (config_.option_helper_.options_
        ->member_tracking.has_member_enableSegmentation
        && config_.option_helper_.options_
        ->member_tracking.member_enableSegmentation) {
      auto img = tracking->QuerySegmentationImage();
      // push into the vector for delayed release
      StoreImageForDelayedRelease(img);
      if (BuildupMaskInfoFromImage(img, false, &data->segmentationMask_))
        data->has_seg_mask_ = true;
    }

    if (config_.option_helper_.options_->member_tracking.has_member_enableBlob
        && config_.option_helper_.options_
            ->member_tracking.member_enableBlob ) {
      auto img = tracking->QueryBlobMask();
      // push into the vector for delayed release
      StoreImageForDelayedRelease(img);
      if (BuildupMaskInfoFromImage(img, true, &data->blobMask_))
        data->has_blob_mask_ = true;
    }
#ifdef PT_MW_DEV
    if (config_.option_helper_.options_->member_tracking.
        member_enablePersonOrientation) {
      auto orient = tracking->QueryPersonOrientation();

      if (orient.confidence) {
        bool valid = true;
        data->personOrientation_.confidence_ = orient.confidence;

        switch (orient.orientation) {
          case TrackingType::ORIENTATION_FRONTAL:
            data->personOrientation_.orientation_ = "frontal";
            break;

          case TrackingType::ORIENTATION_45_DEGREE_RIGHT:
            data->personOrientation_.orientation_ = "45-degree-right";
            break;

          case TrackingType::ORIENTATION_45_DEGREE_LEFT:
            data->personOrientation_.orientation_ = "45-degree-left";
            break;

          case TrackingType::ORIENTATION_PROFILE_RIGHT:
            data->personOrientation_.orientation_ = "profile-right";
            break;

          case TrackingType::ORIENTATION_PROFILE_LEFT:
            data->personOrientation_.orientation_ = "profile-left";
            break;

          case TrackingType::ORIENTATION_REAR:
            data->personOrientation_.orientation_ = "rear";
            break;

          default:
            valid = false;
            break;
        }
        if (valid)
          data->has_orient_ = true;
      }
    }
#endif

    result->track_data_.reset(data);
  }
}

void PersonTrackerAdapter::ExtractGestureData(PTDATA::Person* p,
    PersonDataInternal* result) {
  PTDATA::PersonGestures* ges = p->QueryGestures();
  if (ges) {
    GestureInfo* data = nullptr;
    if (ges->IsPointing()) {
      auto info = ges->QueryPointingInfo();
      data = new GestureInfo;
      data->isPointing_ = true;
      data->has_pointing_info_ = true;
      auto pointInfo = &data->pointingInfo_;
      auto worldInfo = &pointInfo->worldPointingData_;
      auto colorInfo = &pointInfo->colorPointingData_;
      worldInfo->origin_.set_x(info.worldPointingData.origin.x);
      worldInfo->origin_.set_y(info.worldPointingData.origin.y);
      worldInfo->origin_.set_z(info.worldPointingData.origin.z);
      worldInfo->direction_.set_x(info.worldPointingData.direction.x);
      worldInfo->direction_.set_y(info.worldPointingData.direction.y);
      worldInfo->direction_.set_z(info.worldPointingData.direction.z);
      pointInfo->confidence_ = info.confidence;
      pointInfo->gestureStartTimeStamp_ = info.gestureStartTimeStamp;
      colorInfo->origin_.set_x(info.colorPointingData.origin.x);
      colorInfo->origin_.set_y(info.colorPointingData.origin.y);
      colorInfo->direction_.set_x(info.colorPointingData.direction.x);
      colorInfo->direction_.set_y(info.colorPointingData.direction.y);
      result->ges_data_.reset(data);
    }
  }
}

void PersonTrackerAdapter::ExtractPersonFaceData(PTDATA::Person* p,
    PersonDataInternal* result) {
#ifdef PT_MW_DEV
  if (config_.option_helper_.IsExpressionEnabled())
    ExtractExpressionData(p, result);
#endif
  if (config_.option_helper_.IsFaceLandmarkEnabled())
    ExtractLandmarkData(p, result);

  if (config_.option_helper_.IsHeadPoseEnabled()) {
    auto face = p->QueryFace();
    if (face) {
      PTDATA::PoseEulerAngles angles;
      if (face->QueryHeadPose(angles)) {
        result->head_pose_.yaw_ = angles.yaw;
        result->head_pose_.pitch_ = angles.pitch;
        result->head_pose_.roll_ = angles.roll;
        result->has_head_pose_ = true;
      }
    }
  }
}
void PersonTrackerAdapter::ExtractExpressionData(PTDATA::Person* p,
    PersonDataInternal* result) {
#ifdef PT_MW_DEV
  if (p->QueryFace()) {
    auto exprs = p->QueryFace()->QueryExpressions();
    if (exprs) {
      ExpressionType::PersonExpressionsResult ret = {0};

      if (exprs->QueryExpression(ExpressionType::NEUTRAL, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "neutral";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::HAPPINESS, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "happiness";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::SADNESS, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "sadness";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::SURPRISE, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "surprise";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::FEAR, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "fear";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::ANGER, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "anger";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::DISGUST, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "disgust";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }

      if (exprs->QueryExpression(ExpressionType::CONTEMPT, &ret)) {
        ExpressionInfo* info = new ExpressionInfo;

        if (info) {
          info->type_ = "contempt";
          info->confidence_ = ret.confidence;
          result->expression_info_.push_back(info);
        }
      }
    }
  }
#endif
}

void PersonTrackerAdapter::UpdateInternalState(PTDATA* tracking_data) {
  // TODO(shaoting): Call GetTrackingState causes crash when tracking is
  // disabled, avoid calling it before mw fixes this issue.
  if (tracking_data && !config_.option_helper_.IsTrackingDisabled()) {
    switch (tracking_data->GetTrackingState()) {
      case PTDATA::TRACKING_STATE_TRACKING:
        internal_state_ = kStateTracking;
        break;
      case PTDATA::TRACKING_STATE_DETECTING:
        internal_state_ = kStateDetecting;
        break;
      default:
        internal_state_ = kStateError;
        break;
    }
    has_internal_state_update_ = true;
  }
}

void PersonTrackerAdapter::ProcessOutput(PTDATA* tracking_data) {
  UpdateInternalState(tracking_data);
  std::vector<std::shared_ptr<PersonDataInternal>> vec;
  for (int index = 0; index < tracking_data->QueryNumberOfPeople(); ++index) {
    PTDATA::Person* persondata = tracking_data->QueryPersonData(
        PTDATA::ACCESS_ORDER_BY_INDEX, index);

    if (persondata) {
      PersonDataInternal* result = new PersonDataInternal();

      if (config_.option_helper_.IsSkeletonEnabled())
        ExtractSkeletonData(persondata, result);

      if (config_.option_helper_.IsTrackingEnabled()) {
        ExtractTrackData(persondata, result);
      }

      if (config_.option_helper_.IsGestureEnabled())
        ExtractGestureData(persondata, result);

      if (config_.option_helper_.IsExpressionEnabled()
          || config_.option_helper_.IsFaceLandmarkEnabled()
          || config_.option_helper_.IsHeadPoseEnabled())
        ExtractPersonFaceData(persondata, result);

      if (config_.option_helper_.IsPoseEnabled())
        ExtractPoseData(persondata, result);

      vec.push_back(std::shared_ptr<PersonDataInternal>(result));
    }
  }

  SafeGuard guard;
  ShareFrameDataToMainThread();
  if (vec.size()) {
    // in case there are old data that have not been consumed.
    internal_result_.Reset();
    internal_result_.persons_ = vec;
  }
}

void PersonTrackerAdapter::lock() {
  result_mutex_.lock();
}

void PersonTrackerAdapter::unlock() {
  result_mutex_.unlock();
}

bool PersonTrackerAdapter::StartTracking(int32_t track_id) {
  if (!IsRunning())
    return false;
  auto pdata = pt_module_->QueryOutput();
  if (pdata) {
    pdata->StartTracking(track_id);
  }
  return true;
}

bool PersonTrackerAdapter::StopTracking(int32_t track_id) {
  if (!IsRunning())
    return false;
  auto pdata = pt_module_->QueryOutput();
  if (pdata) {
    pdata->StopTracking(track_id);
  }
  return true;
}

std::shared_ptr<PersonDataInternal> PersonTrackerAdapter::GetPersonData(
    int32_t trackID) {
  PTDATA::Person* persondata =
      pt_module_->QueryOutput()->QueryPersonDataById(trackID);
  if (persondata) {
    std::shared_ptr<PersonDataInternal> result =
        std::make_shared<PersonDataInternal>();
    if (config_.option_helper_.IsSkeletonEnabled())
      ExtractSkeletonData(persondata, result.get());
    if (config_.option_helper_.IsTrackingEnabled())
      ExtractTrackData(persondata, result.get());
    if (config_.option_helper_.IsGestureEnabled())
      ExtractGestureData(persondata, result.get());
    if (config_.option_helper_.IsExpressionEnabled()
        || config_.option_helper_.IsFaceLandmarkEnabled()
        || config_.option_helper_.IsHeadPoseEnabled())
      ExtractPersonFaceData(persondata, result.get());
    if (config_.option_helper_.IsPoseEnabled())
      ExtractPoseData(persondata, result.get());
    return result;
  }
  return nullptr;
}

void PersonTrackerAdapter::StoreImageForDelayedRelease(RS::Image* img) {
  if (img) {
    for (auto& old_img : segment_images_) {
      if (img == old_img)
        return;
    }
    segment_images_.push_back(img);
  }
}

void PersonTrackerAdapter::ReleaseSegmentationImages() {
  for (auto& img : segment_images_) {
    img->Release();
  }
  segment_images_.clear();
}

bool PersonTrackerAdapter::RegisterPerson(
    int32_t track_id, PersonRegistrationData* result, std::string* err) {
  // find the Person with the track id.
  PTDATA::Person* persondata =
      pt_module_->QueryOutput()->QueryPersonDataById(track_id);
  if (!persondata) {
    if (err)
      *err = "No person found with this track ID";
    return false;
  }
  // register the person
  PTRECOGNITION* recog = persondata->QueryRecognition();
  if (!recog) {
    if (err)
      *err = "Recognition is not enabled";
    return false;
  }
  PTRECOGNITION::RegistrationStatus status = recog->RegisterUser(
      &result->recognitionID_, &result->trackID_, &result->descriptorID_);
  if (status != PTRECOGNITION::RegistrationSuccessful) {
    if (err)
      *err = GetRegistrationErrDescription(status);
    return false;
  }
  return true;
}

std::string PersonTrackerAdapter::GetRegistrationErrDescription(
    PTRECOGNITION::RegistrationStatus status) {
  switch (status) {
    case PTRECOGNITION::RegistrationFailedAlreadyRegistered:
      return "already registered";
      break;
    case PTRECOGNITION::RegistrationFailedFaceNotDetected:
      return "face not detected";
      break;
    case PTRECOGNITION::RegistrationFailedFaceNotClear:
      return "face not clear";
      break;
    case PTRECOGNITION::RegistrationFailedPersonTooFar:
      return "person too far";
      break;
    case PTRECOGNITION::RegistrationFailedPersonTooClose:
      return "person too close";
      break;
    default:
      return "failed";
      break;
  }
}

bool PersonTrackerAdapter::RecognizePerson(
    int32_t track_id, PersonRecognizerData* result, std::string* err) {
  // find the person with the track id.
  PTDATA::Person* persondata =
      pt_module_->QueryOutput()->QueryPersonDataById(track_id);
  if (!persondata) {
    if (err)
      *err = "no-person-found-with-the-trackID";
    return false;
  }
  // register the person
  PTRECOGNITION* recog = persondata->QueryRecognition();
  if (!recog) {
    if (err)
      *err = "recognition-not-enabled";
    return false;
  }
  PTRECOGNITION::RecognizerData data = {0};
  PTRECOGNITION::RecognitionStatus status = recog->RecognizeUser(&data);
  if (status != PTRECOGNITION::RecognitionPassedPersonRecognized &&
      status != PTRECOGNITION::RecognitionPassedPersonNotRecognized) {
    if (err)
      *err = GetRecognitionErrDescription(status);
    return false;
  }
  result->recognized_ =
      (status == PTRECOGNITION::RecognitionPassedPersonRecognized)?true:false;
  if (result->recognized_) {
    result->trackID_ = data.trackingId;
    result->recognitionID_ = data.recognitionId;
    result->similarityScore_ = data.similarityScore;
  }
  return true;
}

std::string PersonTrackerAdapter::GetRecognitionErrDescription(
    PTRECOGNITION::RecognitionStatus status) {
  switch (status) {
    case PTRECOGNITION::RecognitionPassedPersonRecognized:
      return "recognized";
      break;
    case PTRECOGNITION::RecognitionPassedPersonNotRecognized:
      return "not-recognized";
      break;
    case PTRECOGNITION::RecognitionFailed:
      return "failed";
      break;
    case PTRECOGNITION::RecognitionFailedFaceNotDetected:
      return "face-not-detected";
      break;
    case PTRECOGNITION::RecognitionFailedFaceNotClear:
      return "face-not-clear";
      break;
    case PTRECOGNITION::RecognitionFailedPersonTooFar:
      return "person-too-far";
      break;
    case PTRECOGNITION::RecognitionFailedPersonTooClose:
      return "person-too-close";
      break;
    case PTRECOGNITION::RecognitionFailedFaceAmbiguity:
      return "face-ambiguity";
      break;
    default:
      return "generic-failure";
      break;
  }
}
using PTRecModuleData = PTDATA::RecognitionModuleData;
bool PersonTrackerAdapter::RecognizeAllPersons(
    std::vector<PersonRecognizerDataWithStatus*>* result_vec,
    std::string* err) {
  auto pdata = pt_module_->QueryOutput();
  if (!pdata)
    return false;
  PTRecModuleData* pmodule_data = pdata->QueryRecognitionModuleData();
  if (!pmodule_data) {
    if (err)
      *err = "recognition-not-enabled";
    return false;
  }
  bool success = false;
  int32_t personCount = kDefaultRecognizePersonCount;
  PTRecModuleData::RecognizeAllStatus result =
      PTRecModuleData::RecognizeAllPassed;
  PTRecModuleData::RecognizerDataWithStatus* status_array = NULL;
  int32_t actual_count = 0;
  do {
    if (status_array) {
      delete [] status_array;
      personCount = personCount*2;
    }
    status_array = new PTRecModuleData::RecognizerDataWithStatus[personCount];
    result = pmodule_data->RecognizeAll(
        status_array, personCount, &actual_count);
  } while (result ==
      PTRecModuleData::RecognizeAllUserAllocatedArrayIsSmallerThanExpected);

  if (result == PTRecModuleData::RecognizeAllPassed) {
    if (actual_count) {
      for (int32_t i=0; i < actual_count; i++) {
        auto val = new PersonRecognizerDataWithStatus();
        val->data_.recognized_ = status_array[i].status ==
            PTRECOGNITION::RecognitionPassedPersonRecognized?true:false;
        val->data_.trackID_ = status_array[i].trackingId;
        val->data_.recognitionID_ = status_array[i].recognitionId;
        val->data_.similarityScore_ = status_array[i].similarityScore;
        val->status_ = GetRecognitionErrDescription(status_array[i].status);
        result_vec->push_back(val);
      }
      success = true;
    }
  }

  if (status_array)
    delete [] status_array;
  if (!success && err)
    *err = "not-recognized";
  return success;
}

bool PersonTrackerAdapter::UnRegisterPerson(
    int32_t recognition_id, std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  database->RemoveEntry(recognition_id);
  return true;
}

bool PersonTrackerAdapter::RecognitionIDExist(
    int32_t recognition_id, bool* exist, std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  if (database->Exists(recognition_id))
    *exist = true;
  else
    *exist = false;
  return true;
}

bool PersonTrackerAdapter::GetRecognitionIDs(
    std::vector<int32_t>* id_vec, std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  id_vec->clear();
  int32_t actual_size = 0;
  int32_t id_count = database->GetNumberOfRegisteredUsers();
  if (id_count) {
    int32_t* ids = new int32_t[id_count];
    database->GetRegisteredUsersIds(ids, id_count, &actual_size);
    id_vec->assign(ids, ids+actual_size);
    delete [] ids;
  }
  return true;
}

bool PersonTrackerAdapter::GetRegistrationDescriptorIDs(
    int32_t recognition_id, std::vector<int32_t>* id_vec, std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  id_vec->clear();
  int32_t actual_size = 0;
  int32_t id_count = database->GetNumberOfUserDescriptors(recognition_id);
  bool val = true;
  if (id_count) {
    int32_t* ids = new int32_t[id_count];
    if (!database->GetUserDescriptorIds(recognition_id, ids))
      val = false;
    else
      id_vec->assign(ids, ids+id_count);
    delete [] ids;
  }
  return val;
}

bool PersonTrackerAdapter::RemoveRegistrationDescriptor(
    int32_t recognition_id, int32_t descriptor_id, std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  database->RemoveDescriptor(recognition_id, descriptor_id);
  return true;
}

bool PersonTrackerAdapter::ClearRecognitionDatabase(std::string* err) {
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  if (!database) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  database->Clear();
  return true;
}

bool PersonTrackerAdapter::ExportRecognitionDatabase(
    uint32_t* size, unsigned char** buf, std::string* err) {
  if (!size || !buf)
    return false;
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  auto utils = recogcfg->QueryDatabaseUtilities();
  if (!database || !utils) {
    if (err)
      *err = "can't query the database";
    return false;
  }

  int32_t actual_size = utils->GetDatabaseMemorySize(database);
  int32_t written_size = 0;
  unsigned char* actual_buf = NULL;
  if (actual_size) {
    actual_buf = new unsigned char[actual_size];
    if (!utils->SerializeDatabase(
        database, actual_buf, actual_size, &written_size)) {
      if (err)
        *err = "Failed to export database";
      delete [] actual_buf;
      return false;
    } else {
      if (written_size != actual_size) {
        if (err)
          *err = "Database corrupted";
        delete [] actual_buf;
        return false;
      }
    }
  }
  *size = (uint32_t)written_size;
  *buf = actual_buf;
  return true;
}

bool PersonTrackerAdapter::ImportRecognitionDatabase(
    int32_t size, unsigned char* buf, std::string* err) {
  if (!size || !buf) {
    if (err)
      *err = "Invalid database data for import";
    return false;
  }
  PT::PersonTrackingConfiguration* pt_configuration =
      pt_module_->QueryConfiguration();
  auto recogcfg = pt_configuration->QueryRecognition();
  auto database = recogcfg->QueryDatabase();
  auto utils = recogcfg->QueryDatabaseUtilities();
  if (!database || !utils) {
    if (err)
      *err = "can't query the database";
    return false;
  }
  if (!utils->DeserializeDatabase(database, buf, size)) {
    if (err)
      *err = "Failed to import database";
    return false;
  }
  return true;
}

bool PersonTrackerAdapter::ReinforceRegistration(
    int32_t track_id, int32_t recognition_id, PersonRegistrationData* result,
    std::string* err) {
  // find the Person with the track id.
  PTDATA::Person* persondata =
      pt_module_->QueryOutput()->QueryPersonDataById(track_id);
  if (!persondata) {
    if (err)
      *err = "No person found with this track ID";
    return false;
  }
  // register the person
  PTRECOGNITION* recog = persondata->QueryRecognition();
  if (!recog) {
    if (err)
      *err = "Recognition is not enabled";
    return false;
  }
  int32_t out_track_id = 0;
  int32_t out_descriptor_id = 0;
  PTRECOGNITION::RegistrationStatus status = recog->ReinforceUserRegistration(
      recognition_id, &out_track_id, &out_descriptor_id);
  if (status != PTRECOGNITION::RegistrationSuccessful) {
    if (err)
      *err = GetRegistrationErrDescription(status);
    return false;
  }
  result->trackID_ = out_track_id;
  result->recognitionID_ = recognition_id;
  result->descriptorID_ = out_descriptor_id;
  return true;
}

bool PersonTrackerAdapter::QuerySimilarityScore(
    int32_t track_id, int32_t recognition_id, float* score, std::string* err) {
  // find the person with the track id.
  PTDATA::Person* persondata =
      pt_module_->QueryOutput()->QueryPersonDataById(track_id);
  if (!persondata) {
    if (err)
      *err = "no-person-found-with-the-trackID";
    return false;
  }
  // register the person
  PTRECOGNITION* recog = persondata->QueryRecognition();
  if (!recog) {
    if (err)
      *err = "recognition-not-enabled";
    return false;
  }

  PTRECOGNITION::RecognizerData data = {0};
  PTRECOGNITION::RecognitionStatus status = recog->RecognizeUser(&data);
  if (status != PTRECOGNITION::RecognitionPassedPersonRecognized &&
      status != PTRECOGNITION::RecognitionPassedPersonNotRecognized) {
    if (err)
      *err = GetRecognitionErrDescription(status);
    return false;
  }

  if (data.scoreData) {
    if (score)
      *score = data.scoreData->QuerySimilarityScoreById(recognition_id);
    return true;
  } else {
    if (err)
      *err = "Similarity score data not found";
    return false;
  }
}
