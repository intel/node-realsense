// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "object_recognition_runner.h"

#include <librealsense/rs.hpp>
#include <rs/core/types.h>

#include <memory>
#include <mutex> // NOLINT(*)

#include "camera_runner.h"
#include "localization_info.h"
#include "object_recognition_result_assembler.h"
#include "recognition_info.h"
#include "common/camera-options/camera_options_host_instance.h"
#include "common/frame_data_assembler.h"

#include "gen/nan__frame_data.h"

using rs::object_recognition::or_video_module_impl;
using rs::object_recognition::or_data_interface;
using rs::object_recognition::recognition_data;
using rs::object_recognition::localization_data;
using rs::object_recognition::tracking_data;
using rs::object_recognition::or_configuration_interface;
using rs::core::correlated_sample_set;
using std::unique_ptr;

typedef DictionaryObjectRecognitionOptions DictionaryOROptions;

static void SetDefaultInstanceOptions(DictionaryOROptions* options) {
  auto& or_options = *options;
  or_options.has_member_mode = true;
  or_options.member_mode = "single-recognition";
  // TODO(Kenny): determine other default values
}

class ObjectRecognitionRunnerD : public ObjectRecognitionLabelNameTranslator {
  ObjectRecognitionRunnerD() {
    camera = nullptr;
    or_data = nullptr;
    or_config = nullptr;
    sample_set = nullptr;

    ResetResultPointers();

    state = "<object-recognition-runner-init>";

    camera.reset(new CameraRunner());

    // Default config for object recognition
    SetDefaultInstanceOptions(&or_options);

    skipping_frames_counter = 0;
    last_frame_processed = false;
  }

  virtual ~ObjectRecognitionRunnerD() {
    ObjectRecognitionResultAssembler::SetLabelNameTranslator(nullptr);

    // Begin Note
    //  Do NOT delete the following pointers even if they are not nullptr
    //   (1) recognition_result
    //   (2) localization_result
    //   (3) tracking_result
    // End of Note
  }

  //
  // TODO(Kenny): do the validity check WIDL-NAN tool
  //
  //  For enums, it's easy, just to check strings
  //  For int/float values, we need to find a way to annotate the value range
  //
  static bool CheckObjectRecognitionOptions(
      const DictionaryOROptions& options) {
    bool success = true;

    if (success && options.has_member_mode) {
      if (options.member_mode == "tracking") {
      } else if (options.member_mode == "single-recognition") {
      } else if (options.member_mode == "localization") {
      } else {
        success = false;
      }
    }

    if (success && options.has_member_confidenceThreshold) {
      const auto threshold = options.member_confidenceThreshold;
      success = (threshold > 0.0 && threshold < 1.0);
    }

    if (success && options.has_member_computeEngine) {
      if (options.member_computeEngine == "CPU") {
      } else if (options.member_computeEngine == "GPU") {
      } else {
        success = false;
      }
    }

    // Skip options.has_member_enableSegmentation

    if (success && options.has_member_maxReturnObjectCount) {
      const auto count = options.member_maxReturnObjectCount;
      success = (count > 0);
    }

    // Skip options.has_member_enableObjectCenterEstimation

    // Skip options.has_member_activeClassifier

    // Skip options.has_member_roi

    // Skip options.has_member_trackingRois

    return success;
  }

  // Translate from pure C++ annotation to rs::object_recognition annotation
  bool TranslateObjectRecognitionOptions() {
    bool success = true;
    bool dirty = false;

    if (or_config && or_options.has_member_mode) {
      rs::object_recognition::recognition_mode mode;
      mode = or_config->query_recognition_mode();
      if (or_options.member_mode == "tracking") {
        mode = rs::object_recognition::recognition_mode::TRACKING;
      } else if (or_options.member_mode == "single-recognition") {
        mode = rs::object_recognition::recognition_mode::SINGLE_RECOGNITION;
      } else if (or_options.member_mode == "localization") {
        mode = rs::object_recognition::recognition_mode::LOCALIZATION;
      }

      if (or_config->query_recognition_mode() != mode) {
        dirty = true;
        or_config->set_recognition_mode(mode);
      }
    }

    if (or_config && or_options.has_member_confidenceThreshold) {
      const auto threshold = or_options.member_confidenceThreshold;
      if (or_config->query_recognition_confidence() != threshold) {
        or_config->set_recognition_confidence(threshold);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_computeEngine) {
      rs::object_recognition::compute_engine engine;
      engine = or_config->query_compute_engine();
      if (or_options.member_computeEngine == "CPU") {
        engine = rs::object_recognition::compute_engine::CPU;
      } else if (or_options.member_computeEngine == "GPU") {
        engine = rs::object_recognition::compute_engine::GPU;
      }

      if (or_config->query_compute_engine() != engine) {
        or_config->set_compute_engine(engine);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_enableSegmentation) {
      const auto enabled = or_options.member_enableSegmentation;
      if (or_config->is_segmentation_enabled() != enabled) {
        or_config->enable_segmentation(enabled);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_maxReturnObjectCount) {
      const auto count = or_options.member_maxReturnObjectCount;
      if (or_config->query_max_return_probs_count() != count) {
        or_config->set_max_return_probs_count(count);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_enableObjectCenterEstimation) {
      const auto enabled = or_options.member_enableObjectCenterEstimation;
      if (or_config->is_object_center_estimation_enabled() != enabled) {
        or_config->enable_object_center_estimation(enabled);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_activeClassifier) {
      const auto& classifier = or_options.member_activeClassifier;
      if (classifier != or_config->query_active_classifier()) {
        or_config->set_active_classifier(classifier.c_str());
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_roi) {
      const auto new_roi = or_options.member_roi;
      rs::core::rect roi = or_config->query_roi();

      bool same = (new_roi.get_x() == roi.x &&
          new_roi.get_y() == roi.y &&
          new_roi.get_width() == roi.width &&
          new_roi.get_height() == roi.height);

      if (!same) {
        or_config->set_roi(roi);
        dirty = true;
      }
    }

    if (or_config && or_options.has_member_trackingRois) {
      auto array = new rs::core::rect[or_options.member_trackingRois_size_];
      for (size_t i = 0; i < or_options.member_trackingRois_size_; ++i) {
        array[i].x = or_options.member_trackingRois[i].get_x();
        array[i].y = or_options.member_trackingRois[i].get_y();
        array[i].width = or_options.member_trackingRois[i].get_width();
        array[i].height = or_options.member_trackingRois[i].get_height();
      }

      int array_size = static_cast<int>(or_options.member_trackingRois_size_);
      or_config->set_tracking_rois(array, array_size);
      // TODO(Kenny): to determine whether it's truely dirty or not
      //              e.g. by caching the value we've set last time
      dirty = true;
    }

    if (dirty)
      success = (or_config->apply_changes() == rs::core::status_no_error);

    return success;
  }

  bool ProcessObjectRecognition() {
    bool success = true;
    rs::core::status st = rs::core::status_no_error;

    st = or_impl->process_sample_set(*sample_set);
    success = success && (st == rs::core::status_no_error);

    if (success) {
      if (or_options.member_mode == "tracking") {
        st = or_data->query_tracking_result(&tracking_result,
                                            tracking_result_size);
      } else if (or_options.member_mode == "single-recognition") {
        st = or_data->query_single_recognition_result(&recognition_result,
                                                      recognition_result_size);
      } else if (or_options.member_mode == "localization") {
        st = or_data->query_localization_result(&localization_result,
                                                localization_result_size);
      }
    }

    return success;
  }

  std::string GetEventName() const {
    if (or_options.member_mode == "tracking") {
      return "tracking";
    } else if (or_options.member_mode == "single-recognition") {
      return "recognition";
    } else if (or_options.member_mode == "localization") {
      return "localization";
    }
    return "<invalid-event-name>";
  }

  v8_value_t AssembleResult() {
    v8_value_t value = Nan::Undefined();

    if (!ObjectRecognitionResultAssembler::GetLabelNameTranslator()) {
      ObjectRecognitionResultAssembler::SetLabelNameTranslator(this);
    }
    if (or_options.member_mode == "tracking") {
      value = ObjectRecognitionResultAssembler::AssembleTrackingResult(
          tracking_result,
          tracking_result_size);
    } else if (or_options.member_mode == "single-recognition") {
      value = ObjectRecognitionResultAssembler::AssembleSingleRecognitionResult(
          recognition_result,
          recognition_result_size);
    } else if (or_options.member_mode == "localization") {
      value = ObjectRecognitionResultAssembler::AssembleLocalizationResult(
          localization_result,
          localization_result_size);
    }

    ResetResultPointers();

    return value;
  }

  bool HasResult() const {
    if (or_options.member_mode == "tracking") {
      return nullptr != tracking_result;
    } else if (or_options.member_mode == "single-recognition") {
      return nullptr != recognition_result;
    } else if (or_options.member_mode == "localization") {
      return nullptr != localization_result;
    }
    return false;
  }

  bool HasSkippingFramesOption() const {
    return or_options.has_member_framesSkipping &&
      or_options.member_framesSkipping > 0;
  }

  bool IsSkippingFramesCounterOverflow() const {
    return skipping_frames_counter > or_options.member_framesSkipping;
  }

  void NudgeSkippingFramesCounter() {
    ++skipping_frames_counter;
  }

  void ResetSkippingFramesCounter() {
    skipping_frames_counter = 0;
  }

  void ResetResultPointers() {
    tracking_result = nullptr;
    tracking_result_size = 0;

    localization_result = nullptr;
    recognition_result_size = 0;

    recognition_result = nullptr;
    localization_result_size = 0;
  }

  virtual std::string DoTranslate(size_t id) {
    if (or_config)
      return or_config->query_object_name_by_id(id);

    return "<config-obj-not-found>";
  }

  v8_value_t GetFrameData() {
    if (assembler) {
      auto result = NanFrameData::NewInstance(assembler->ToFrameData());
      assembler.reset();
      return result;
    }

    return Nan::Undefined();
  }

  DictionaryOROptions             or_options;
  DictionaryCameraOptions         camera_options;

  recognition_data*               recognition_result;
  int                             recognition_result_size;
  localization_data*              localization_result;
  int                             localization_result_size;
  tracking_data*                  tracking_result;
  int                             tracking_result_size;


  // Be careful: this `camera_utils` is a fast changing util
  // The ultimate goal is to remove this util and use our own
  unique_ptr<CameraRunner>        camera;
  rs::core::image_info            colorInfo;
  rs::core::image_info            depthInfo;
  unique_ptr<or_video_module_impl>or_impl;
  or_data_interface*              or_data;
  or_configuration_interface*     or_config;
  correlated_sample_set*          sample_set;

  std::mutex                      sample_set_mutex;
  int32_t                         skipping_frames_counter;
  bool                            last_frame_processed;

  std::string                     state;

  unique_ptr<FrameDataAssembler>  assembler;

  friend class ObjectRecognitionRunner;
};


ObjectRecognitionRunner::ObjectRecognitionRunner() {
  d_ = new ObjectRecognitionRunnerD();
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  host->GetCameraOptionsIO()->Add(this);
}

ObjectRecognitionRunner::~ObjectRecognitionRunner() {
  delete d_;
}

std::string ObjectRecognitionRunner::GetInstanceState() const {
  return d_->state;
}

void ObjectRecognitionRunner::SetInstanceState(const std::string& state) {
  d_->state = state;
}

bool ObjectRecognitionRunner::ImportOptions(
    const DictionaryOROptions& options) {
  bool success = true;

  if (success) {
    // To check if the options are valid
    success = d_->CheckObjectRecognitionOptions(options);
  }

  if (success) {
    // Only overwrite by the members that are present in `options`
    d_->or_options.IncrementallyCopyFrom(options);
    success = d_->TranslateObjectRecognitionOptions();
  }

  return success;
}

void ObjectRecognitionRunner::ExportOptions(
    DictionaryOROptions* options) {
  options->CopyFrom(d_->or_options);
}

bool ObjectRecognitionRunner::Init() {
  bool success = true;

  if (!d_->or_impl)
    d_->or_impl.reset(new or_video_module_impl());

  if (success) {
    success = d_->camera->StartCamera(d_->colorInfo, d_->depthInfo,
                                      *d_->or_impl.get(),
                                      &d_->or_data,
                                      &d_->or_config);
    d_->ResetSkippingFramesCounter();
  }

  if (success)
    success = d_->TranslateObjectRecognitionOptions();

  return success;
}

bool ObjectRecognitionRunner::IsLastFrameProcessed() const {
  return d_->last_frame_processed;
}

bool ObjectRecognitionRunner::Run() {
  std::unique_lock<std::mutex> lock(d_->sample_set_mutex);

  try {
    d_->sample_set = d_->camera->GetSampleSet(d_->colorInfo, d_->depthInfo);

    if (d_->HasSkippingFramesOption())
      d_->NudgeSkippingFramesCounter();

    d_->last_frame_processed = true;
    if (d_->HasSkippingFramesOption())
      d_->last_frame_processed = d_->IsSkippingFramesCounterOverflow();
    if (d_->last_frame_processed)
      d_->ProcessObjectRecognition();

    if (d_->HasSkippingFramesOption() &&
        d_->IsSkippingFramesCounterOverflow()) {
      d_->ResetSkippingFramesCounter();
    }

    d_->assembler.reset(new FrameDataAssembler());

    rs::core::image_interface* color_img =
        (*d_->sample_set)[rs::core::stream_type::color];
    rs::core::image_interface* depth_img =
        (*d_->sample_set)[rs::core::stream_type::depth];

    d_->assembler->SetData(color_img);
    d_->assembler->SetData(depth_img);
  } catch (const rs::error& error) {
    throw std::string(error.what());
  }

  return true;
}

bool ObjectRecognitionRunner::RetrieveCameraFrame() {
  return true;
}

bool ObjectRecognitionRunner::ProcessCameraFrame() {
  return true;
}

void ObjectRecognitionRunner::GetCameraFrame() {
}

std::string ObjectRecognitionRunner::GetEventName() const {
  return d_->GetEventName();
}

bool ObjectRecognitionRunner::IsResultReady() const {
  return d_->state == OR_STATE_RUNNING && d_->HasResult();
}

v8_value_t ObjectRecognitionRunner::GetResult() {
  return d_->AssembleResult();
}

v8_value_t ObjectRecognitionRunner::GetFrameData() {
  return d_->GetFrameData();
}

void ObjectRecognitionRunner::Stop() {
  d_->camera->StopCamera();  // Keep camera object to receive options
  d_->or_impl.reset();  // Release more resource
  d_->ResetResultPointers();
  d_->ResetSkippingFramesCounter();
}

void ObjectRecognitionRunner::Reset() {
  Cleanup();
  delete d_;
  d_ = new ObjectRecognitionRunnerD();
  SetInstanceState(OR_STATE_READY);
}

void ObjectRecognitionRunner::Cleanup() {
  Stop();
}

bool ObjectRecognitionRunner::IsSupported(const CameraOptionsType* src) const {
  return CameraRunner::IsSupportedConfig(*src);
}

std::string ObjectRecognitionRunner::GetUnsupportedMessage() const {
  const char* full_msg =
  "You've set CameraOptions to Object Recognition that are not currently supported." "\n" // NOLINT
  "Try set one of the options in below list:" "\n"
  "-----------------------------------------" "\n"
  "color: 1920x1080, depth: 628x468, fps: 30" "\n"
  "color: 1920x1080, depth: 320x240, fps: 30" "\n"
  "color: 1920x1080, depth: 480x360, fps: 30" "\n"
  "color: 640x480,   depth: 628x468, fps: 30" "\n"
  "color: 640x480,   depth: 640x480, fps: 30" "\n"
  "color: 640x480,   depth: 320x240, fps: 30" "\n"
  "-----------------------------------------" "\n";

  return full_msg;
}

void ObjectRecognitionRunner::ApplyToCamera(const CameraOptionsType* src) {
  if (src)
    d_->camera->SetExpectedCameraConfig(*src);
}

void ObjectRecognitionRunner::PartiallyFillInCameraData(CameraOptionsType* target) {  // NOLINT(*)
}

bool ObjectRecognitionRunner::DumpRecognizableObjectNames(
    std::vector<std::string>* target) {
  if (target && d_->camera && d_->or_config) {
    d_->camera->QueryObjectNameList(*target, d_->or_config);
    return true;
  }
  return false;
}

static ObjectRecognitionRunner* g_Instance = nullptr;

ObjectRecognitionRunner* ObjectRecognitionRunner::GetRunnerInstance() {
  if (!g_Instance)
    g_Instance = new ObjectRecognitionRunner();

  return g_Instance;
}

void ObjectRecognitionRunner::DestroyRunnerInstance() {
  delete g_Instance;
  g_Instance = nullptr;
}
