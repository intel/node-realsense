// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "camera_runner.h"
#include "common/camera-options/camera_options_coprocessor.h"

#include <fstream>

CameraRunner::CameraRunner() {
  sample_set_ = nullptr;
  device_ = nullptr;
  color_buffer_ = nullptr;
  camera_running_ = false;
  last_error_ = rs::core::status_no_error;
}

CameraRunner::~CameraRunner() {
}

bool CameraRunner::StartCamera(image_info& colorInfo,  // NOLINT(*)
    image_info& depthInfo,  // NOLINT(*)
    or_video_module_impl& impl,  // NOLINT(*)
    or_data_interface** or_data,
    or_configuration_interface** or_configuration) {
  rs::core::status st = rs::core::status_no_error;
  frame_number_ = 0;

  ctx_ = CameraDelegateInstance::GetInstance();

  // Check if there is request to use playback file as virtual camera
  if (expected_camera_config_.has_member_playbackPathForTesting) {
    const char* playback_filename =
        expected_camera_config_.member_playbackPathForTesting.c_str();
    std::ifstream playback_file(playback_filename);
    if (playback_file.good())
      ctx_->ConnectToVirtualCamera(playback_filename);
  }

  if (ctx_ == nullptr) {
    last_error_ = status_device_lost;
    return false;
  }

  if (ctx_->get_device_count() == 0) {
    last_error_ = status_device_lost;
    return false;
  }

  // Get pointer to the camera
  device_ = ctx_->get_device(0);

  // Request the first (index 0) supproted module config.
  rs::core::video_module_interface::supported_module_config cfg;
  QuerySupportedConfig(impl, cfg);
  if (st != rs::core::status_no_error) {
    last_error_ = st;
    return false;
  }

  color_width_ = cfg.image_streams_configs[color_stream].size.width;
  color_height_ = cfg.image_streams_configs[color_stream].size.height;

  // Enables streams according to the supported configuration
  device_->enable_stream(rs::stream::color,
             cfg.image_streams_configs[color_stream].size.width,
             cfg.image_streams_configs[color_stream].size.height,
             rs::format::rgb8,
             cfg.image_streams_configs[color_stream].frame_rate);

  device_->enable_stream(rs::stream::depth,
             cfg.image_streams_configs[depth_stream].size.width,
             cfg.image_streams_configs[depth_stream].size.height,
             rs::format::z16,
             cfg.image_streams_configs[depth_stream].frame_rate);

  // Handling color image info (for later using)
  colorInfo.height = cfg.image_streams_configs[color_stream].size.height;
  colorInfo.width = cfg.image_streams_configs[color_stream].size.width;
  colorInfo.format = rs::core::pixel_format::rgb8;
  colorInfo.pitch = colorInfo.width * 3;
  color_info_ = colorInfo;

  // Handling depth image info (for later using)
  depthInfo.height = cfg.image_streams_configs[depth_stream].size.height;
  depthInfo.width = cfg.image_streams_configs[depth_stream].size.width;
  depthInfo.format = rs::core::pixel_format::z16;
  depthInfo.pitch = depthInfo.width * 2;

  CameraOptionsCoProcessor cop(expected_camera_config_,
    CameraOptionsCoProcessor::ObjectRecognition);
  cop.TryEnableExtraChannels(device_);
  cop.TrySetExtraOptions(device_);

  // Enable auto exposure for color stream
  device_->set_option(rs::option::color_enable_auto_exposure, 1);

  // Enable auto exposure for Depth camera stream
  device_->set_option(rs::option::r200_lr_auto_exposure_enabled, 1);

  device_->start(cop.GetCameraStartOptions(device_));

  // Get the extrisics paramters from the camera
  rs::extrinsics ext  = device_->get_extrinsics(rs::stream::depth,
      rs::stream::color);
  rs::core::extrinsics core_ext;

  // Get color intrinsics
  rs::intrinsics colorInt = device_->get_stream_intrinsics(rs::stream::color);
  rs::core::intrinsics core_colorInt;

  // Get depth intrinsics
  rs::intrinsics depthInt = device_->get_stream_intrinsics(rs::stream::depth);
  rs::core::intrinsics core_depthInt;

  // After getting all parameters from the camera we need
  //  to set the actual_module_config
  rs::core::video_module_interface::actual_module_config actualConfig;

  // 1. copy the extrinsics
  memcpy(&actualConfig.image_streams_configs[color_stream].extrinsics,
      &ext, sizeof(rs::extrinsics));
  core_ext =  rs::utils::convert_extrinsics(ext);

  // 2. copy the color intrinsics
  memcpy(&actualConfig.image_streams_configs[color_stream].intrinsics,
      &colorInt, sizeof(rs::intrinsics));
  core_colorInt = rs::utils::convert_intrinsics(colorInt);

  // 3. copy the depth intrinsics
  memcpy(&actualConfig.image_streams_configs[depth_stream].intrinsics,
      &depthInt, sizeof(rs::intrinsics));
  core_depthInt = rs::utils::convert_intrinsics(depthInt);

  // Handling projection
  rs::core::projection_interface* proj =
      rs::core::projection_interface::create_instance(&core_colorInt,
      &core_depthInt,
      &core_ext);
  actualConfig.projection = proj;

  // Setting the selected configuration (after projection)
  st = impl.set_module_config(actualConfig);
  if (st != rs::core::status_no_error) {
    last_error_ = st;
    return false;
  }

  // Create or data object
  *or_data = impl.create_output();

  // Create or data object
  *or_configuration = impl.create_active_configuration();

  TouchSampleSet();

  camera_running_ = true;

  return true;
}

// Get rs::core::correlated_sample_set* from the camrea
//  (encapsulates all conversion staff)
correlated_sample_set* CameraRunner::GetSampleSet(image_info& colorInfo,  // NOLINT(*)
    image_info& depthInfo) {  // NOLINT(*)
  device_->wait_for_frames();

  // Get color and depth buffers
  const void* colorBuffer =  device_->get_frame_data(rs::stream::color);
  const void* depthBuffer = device_->get_frame_data(rs::stream::depth);
  color_buffer_ = const_cast<void*>(colorBuffer);

  TouchSampleSet();

  // Release images from the prevoius frame
  ReleaseImages();

  // Create images from buffers
  typedef rs::core::image_interface::image_data_with_data_releaser release_t;
  release_t color_container(colorBuffer);
  release_t depth_container(depthBuffer);
  auto colorImg = rs::core::image_interface::create_instance_from_raw_data(
      &colorInfo, color_container,
      rs::core::stream_type::color,
      rs::core::image_interface::any,
      static_cast<uint64_t>(device_->get_frame_timestamp(rs::stream::color)),
      frame_number_);
  auto depthImg = rs::core::image_interface::create_instance_from_raw_data(
      &depthInfo, depth_container,
      rs::core::stream_type::depth,
      rs::core::image_interface::any,
      static_cast<uint64_t>(device_->get_frame_timestamp(rs::stream::depth)),
      frame_number_);

  // Create sample from both images
  sample_set_->images[color_stream] = colorImg;
  sample_set_->images[depth_stream] = depthImg;
  ++frame_number_;

  return sample_set_;
}

void CameraRunner::TouchSampleSet() {
  if (!sample_set_) {
    sample_set_ = new rs::core::correlated_sample_set();
    sample_set_->images[color_stream] = nullptr;
    sample_set_->images[depth_stream] = nullptr;
  }
}

void CameraRunner::StopCamera() {
  if (camera_running_) {
    device_->stop();
    camera_running_ = false;
  }

  delete device_;
  device_ = nullptr;
  ctx_ = nullptr;  // No need to delete singleton object
  ReleaseImages();
  delete sample_set_;
  sample_set_ = nullptr;
}

bool CameraRunner::IsSupportedConfig(const CameraOptionsType& options) {
  if (!options.has_member_color || !options.has_member_depth) {
    return false;
  }
  // I don't care .has_member_fisheye in OR algorithm
  // I don't care .has_member_accel in OR algorithm
  // I don't care .has_member_gyro in OR algorithm

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
      !depth_stream.member_isEnabled) {
    return false;
  }

  or_video_module_impl impl;
  video_module_interface::supported_module_config config;
  for (int i = 0; ; ++i) {
    rs::core::status st = impl.query_supported_module_config(i, config);
    if (st != rs::core::status_no_error)
      return false;

    auto& color = config.image_streams_configs[CameraRunner::color_stream];
    auto& depth = config.image_streams_configs[CameraRunner::depth_stream];
    auto& expected = options;

    if (color.size.width == expected.member_color.member_width &&
        color.size.height == expected.member_color.member_height &&
        color.frame_rate == expected.member_color.member_frameRate &&

        depth.size.width == expected.member_depth.member_width &&
        depth.size.height == expected.member_depth.member_height &&
        depth.frame_rate == expected.member_depth.member_frameRate ) {
      // Found it
      return true;
    }
  }

  return false;
}

// NOLINT(*)
int CameraRunner::QuerySupportedConfig(or_video_module_impl& or_impl,  // NOLINT(*)
    video_module_interface::supported_module_config& config) {  // NOLINT(*)

  for (int i = 0; ; ++i) {
    rs::core::status st = or_impl.query_supported_module_config(i, config);
    if (st != rs::core::status_no_error) {
      last_error_ = st;
      return 0;
    }

    const auto& color = config.image_streams_configs[color_stream];
    const auto& depth = config.image_streams_configs[depth_stream];
    const auto& expected = expected_camera_config_;

    if (color.size.width == expected.member_color.member_width &&
        color.size.height == expected.member_color.member_height &&
        color.frame_rate == expected.member_color.member_frameRate &&

        depth.size.width == expected.member_depth.member_width &&
        depth.size.height == expected.member_depth.member_height &&
        depth.frame_rate == expected.member_depth.member_frameRate ) {
      // Found it
      return 0;
    }
  }

  return -1;
}

void CameraRunner::QueryObjectNameList(std::vector<std::string>& obj_name_list,  // NOLINT(*)
    rs::object_recognition::or_configuration_interface* or_configuration) {
  int i = 0;
  while (1) {
    std::string objName = or_configuration->query_object_name_by_id(i);
    if (i > 50 || objName.empty()) {
      break;
    } else {
      obj_name_list.push_back(objName);
      i++;
    }
  }
}

void CameraRunner::ReleaseImages() {
  if (sample_set_) {
    if (sample_set_->images[color_stream]) {
      sample_set_->images[color_stream]->release();
      sample_set_->images[color_stream] = nullptr;
    }

    if (sample_set_->images[depth_stream]) {
      sample_set_->images[depth_stream]->release();
      sample_set_->images[depth_stream] = nullptr;
    }
  }
}

std::string CameraRunner::GetLastErrorInfo() const {
  // TODO(Kenny): translate this->last_error_ into human readable message
  // This will be used for better error/exception/rejection message
  return "";
}
