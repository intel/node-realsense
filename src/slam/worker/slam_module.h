// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_SLAM_MODULE_H_
#define _WORKER_SLAM_MODULE_H_

#include <rs_sdk.h>

#include <memory>
#include <string>

#include "async_channel.h"
#include "gen/instance_options.h"
#include "occupancy_map_data.h"
#include "map_image.h"
#include "occupancy_map_bounds.h"
#include "slam_handlers.h"
#include "utils.h"

class SlamModule {
 public:
  SlamModule() : state_(SlamState::kIdle),
                 process_handler_(nullptr),
                 control_handler_(nullptr) {
    occupancy_map_.reset();
  }

  ~SlamModule() {
    slam_.reset();
    if (process_handler_ != nullptr) delete process_handler_;

    if (control_handler_ != nullptr) delete control_handler_;

    channel_.reset();
    occupancy_map_.reset();
  }

  void set_channel(std::shared_ptr<AsyncChannel> channel) {
    channel_ = channel;
    if (process_handler_ != nullptr) process_handler_->set_channel(channel);

    if (control_handler_ != nullptr) control_handler_->set_channel(channel);
  }

  utils::Status Init();
  utils::Status Start();
  // TODO(Donna): support utils::Status reset();
  utils::Status Stop();
  utils::Status Destroy();


  utils::Status RestartTracking();

  SlamState state();
  InternalCameraConfig camera_config();
  utils::Status ApplyCameraConfig(const InternalCameraConfig& camera_config);
  DictionaryInstanceOptions module_config();
  utils::Status ApplyModuleConfig(
      const DictionaryInstanceOptions& module_config);
  utils::Status QueryCurrentModuleConfig();
  OutputHolder* MoveTrackingResult() {
    return process_handler_->MoveResult();
  }
  utils::Status GetOccupancyMapUpdate(OccupancyMapData* map_data);
  utils::Status GetOccupancyMapAsRgba(
      MapImage* map_image,
      bool draw_pose_trajectory,
      bool draw_occupancy_map);
  utils::Status GetOccupancyMapBounds(OccupancyMapBounds* map_bounds);
  utils::Status SaveOccupancyMapAsPpm(
      const std::string& file_name, bool draw_camera_trajectory);
  utils::Status SaveOccupancyMap(const std::string& file_name);

 private:
  utils::Status QuerySupportedConfig();
  utils::Status SetStreamCallbacks();
  utils::Status SetMotionCallbacks();
  utils::Status ApplyCameraConfig(
      const InternalCameraConfig& camera_config, bool from_js_request);
  utils::Status ApplyModuleConfig(
      const DictionaryInstanceOptions& module_config, bool from_js_request);

  // These should only be accessible to worker.
  rs::context rs_context_;
  rs::device* device_;
  std::shared_ptr<rs::slam::slam> slam_;

  rs::core::video_module_interface::supported_module_config
      supported_config_ = {};

  rs::core::video_module_interface::actual_module_config actual_config_ = {};

  rs::source active_sources_;

  InternalCameraConfig camera_config_;
  DictionaryInstanceOptions module_config_;
  SlamState state_;
  std::shared_ptr<AsyncChannel> channel_;
  std::shared_ptr<rs::slam::occupancy_map> occupancy_map_;

  SlamEventHandler* process_handler_;
  SlamTrackingEventHandler* control_handler_;
};

#endif  // _WORKER_SLAM_MODULE_H_
