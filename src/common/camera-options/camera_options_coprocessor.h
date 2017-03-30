// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <librealsense/rs.hpp>

#include "common/camera-options/camera_options_type.h"

class CameraOptionsCoProcessor {
 public:
  enum Caller {
    ObjectRecognition,
    PersonTracking,
    SLAM
  };

  CameraOptionsCoProcessor(const CameraOptionsType& cameraOptions,
      Caller caller) {
    prev_start_source_ = rs::source::video;
    motion_events_enabled_ = false;
    fisheye_request_ = false;

    expected_camera_config_ = cameraOptions;
    caller_ = caller;

    const auto& expected = expected_camera_config_;
    if (expected.has_member_fisheye) {
      const auto& fisheye = expected.member_fisheye;
      if (fisheye.has_member_isEnabled && fisheye.member_isEnabled) {
        // Yes, we're probably working with SLAM add-on
        fisheye_request_ = true;
      }
    }

    if (expected.has_member_color) {
      const auto& color =  expected.member_color;
      if (color.has_member_isEnabled && color.member_isEnabled) {
        // Yes, we're probably working with SLAM add-on
        color_request_ = true;
      }
    }
  }

  void TryEnableExtraChannels(CameraDelegateDevice* device) {
    if (caller_ == Caller::SLAM) {
      if (color_request_) {
        try {
          const auto& color = expected_camera_config_.member_color;
          device->enable_stream(rs::stream::color,
            color.member_width,
            color.member_height,
            rs::format::rgb8,
            color.member_frameRate);
        } catch(...) {
          // Absort all exceptions
        }
      }
    } else {
      if (fisheye_request_ && device->supports(rs::capabilities::fish_eye)) {
        try {
          const auto& fisheye = expected_camera_config_.member_fisheye;
          device->enable_stream(rs::stream::fisheye,
            fisheye.member_width,
            fisheye.member_height,
            rs::format::raw8,
            fisheye.member_frameRate);
        } catch(...) {
          // Absort all exceptions
        }
      }
    }
  }

  void TrySetExtraOptions(CameraDelegateDevice* device) {
    if (caller_ == Caller::SLAM) {
      if (color_request_) {
        // Working with PT/OR add-on
        device->set_option(rs::option::color_enable_auto_exposure, 1);
      }
    } else {
      if (fisheye_request_) {
        if (device->supports(rs::capabilities::fish_eye)) {
          device->set_option(rs::option::fisheye_strobe, 1);
          device->set_option(rs::option::fisheye_color_auto_exposure, 1);
        }

        if (device->supports(rs::capabilities::motion_events)) {
          motion_events_enabled_ = true;

          // CameraDelegateDevice will maintain an internal list of callbacks
          device->enable_motion_tracking([](rs::motion_data){
            // NOP
          }, [](rs::timestamp_data){
            // NOP
          });
        }
      }
    }
  }

  void TryUnsetExtraOptions(CameraDelegateDevice* device) {
    if (motion_events_enabled_) {
      motion_events_enabled_ = false;
      device->disable_motion_tracking();
    }
  }

  rs::source GetCameraStartOptions(CameraDelegateDevice* device) const {
    prev_start_source_ = rs::source::video;
    if (caller_ == Caller::SLAM) {
      prev_start_source_ = rs::source::all_sources;
    } else {
      if (fisheye_request_) {
        if (device->supports(rs::capabilities::motion_events)) {
          // Only in this case we consider request for SLAM addon
          prev_start_source_ = rs::source::all_sources;
        }
      }
    }
    return prev_start_source_;
  }

  rs::source GetPreviousCameraStartOptions() const {
    return prev_start_source_;
  }

 private:
  CameraOptionsType expected_camera_config_;
  Caller caller_;
  bool   fisheye_request_;
  bool   color_request_;

  mutable rs::source prev_start_source_;
  bool   motion_events_enabled_;
};
