// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_RUNNER_H_
#define _CAMERA_RUNNER_H_

#include <librealsense/rs.hpp>
#include <or_data_interface.h>
#include <or_configuration_interface.h>
#include <or_video_module_impl.h>
#include <rs_sdk.h>

#include <iostream>
#include <memory>
#include <vector>
#include <string>

#include "common/camera-options/camera_options_type.h"
#include "common/camera-delegate/camera_delegate_instance.h"

using namespace rs::core;  // NOLINT(*)
using namespace rs::object_recognition;  // NOLINT(*)

class CameraRunner {
 public:
  CameraRunner();
  ~CameraRunner();

  static const int color_stream = static_cast<int>(rs::stream::color);
  static const int depth_stream = static_cast<int>(rs::stream::depth);

  // Handling all camera initilzation (projection,
  //  camera configuration, MW intilization)
  bool StartCamera(image_info& colorInfo,  // NOLINT(*)
      image_info& depthInfo,  // NOLINT(*)
      or_video_module_impl& impl,  // NOLINT(*)
      or_data_interface** or_data,
      or_configuration_interface** or_configuration);

  // Get rs::core::correlated_sample_set* from the camrea
  //  (encapsulates all conversion staff)
  correlated_sample_set* GetSampleSet(image_info& colorInfo,  // NOLINT(*)
      image_info& depthInfo);  // NOLINT(*)

  bool IsCameraRunning() const { return camera_running_; }

  // Stop the device
  void StopCamera();

  inline int GetFrameNumber() const {
    return frame_number_;
  }

  inline int GetColorWidth() const {
    return color_width_;
  }

  inline int GetColorHeight() const {
    return color_height_;
  }

  // NOLINT(*)
  int QuerySupportedConfig(or_video_module_impl& or_impl,  // NOLINT(*)
      video_module_interface::supported_module_config& supported_config);  // NOLINT(*)

  void QueryObjectNameList(std::vector<std::string>& obj_name_list,  // NOLINT(*)
      rs::object_recognition::or_configuration_interface* or_configuration);

  inline void SetExpectedCameraConfig(const CameraOptionsType& options) {
    expected_camera_config_ = options;
  }

  inline const CameraOptionsType& GetExpectedCameraConfig() const {
    return expected_camera_config_;
  }

  static bool IsSupportedConfig(const CameraOptionsType&);

  std::string GetLastErrorInfo() const;

 private:
  // std::shared_ptr<rs::core::context_interface> ctx_;
  // rs::device* device_;
  CameraDelegate* ctx_;
  CameraDelegateDevice* device_;

  rs::core::correlated_sample_set* sample_set_;
  rs::core::image_info color_info_;

  void* color_buffer_;
  int frame_number_;

  int color_width_;
  int color_height_;

  CameraOptionsType expected_camera_config_;

  bool camera_running_;

  void ReleaseImages();
  void TouchSampleSet();

  rs::core::status last_error_;
};

#endif  // _CAMERA_RUNNER_H_
