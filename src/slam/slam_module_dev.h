// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_SLAM_MODULE_DEV_H_
#define _WORKER_SLAM_MODULE_DEV_H_

#include <rs_sdk.h>

#include <memory>
#include <string>

#include "common/camera-options/camera_options_host_instance.h"
#include "common/camera-options/camera_options_io.h"
#include "gen/camera_options.h"
#include "gen/instance_options.h"
#include "occupancy_map_data.h"
#include "map_image.h"
#include "occupancy_map_bounds.h"
#include "slam_handlers.h"
#include "utils.h"

class SlamModuleDev : public CameraOptionsIO {
 public:
  SlamModuleDev() {
    CleanUp();
    RegisterToCameraHost();
  }

  ~SlamModuleDev() {
     CleanUp();
  }

  utils::Status Start();
  utils::Status Reset();
  utils::Status Stop();
  utils::Status RestartTracking();

  SlamState state();
  utils::Status ExportModuleConfig(DictionaryInstanceOptions* module_config);
  utils::Status ApplyModuleConfig(const DictionaryInstanceOptions& external);
  utils::Status QueryCurrentModuleConfig();
  OutputHolder* MoveTrackingResult();
  utils::Status GetOccupancyMapUpdate(OccupancyMapData* map_data);
  utils::Status GetOccupancyMapAsRgba(
      MapImage* map_image,
      bool draw_pose_trajectory,
      bool draw_occupancy_map);
  utils::Status GetOccupancyMapBounds(OccupancyMapBounds* map_bounds);
  utils::Status SaveOccupancyMapAsPpm(
      const std::string& file_name, bool draw_camera_trajectory);
  utils::Status SaveOccupancyMap(const std::string& file_name);
  utils::Status LoadOccupancyMap(const std::string& file_name);

  // CameraOptionsIO methods
  bool IsSupported(const CameraOptionsType* src) const override;
  std::string GetUnsupportedMessage() const override;
  void ApplyToCamera(const CameraOptionsType* src) override;
  void PartiallyFillInCameraData(CameraOptionsType* target) override;

 private:
  utils::Status Init();
  void CleanUp();
  void RegisterToCameraHost();
  utils::Status StopDevice();
  utils::Status QuerySupportedConfig();
  utils::Status SetStreamCallbacks();
  utils::Status SetMotionCallbacks();

  // These should only be accessible to worker.
  rs::context rs_context_;
  rs::device* device_;
  std::shared_ptr<rs::slam::slam> slam_;

  rs::core::video_module_interface::supported_module_config
      supported_config_ = {};

  rs::core::video_module_interface::actual_module_config actual_config_ = {};

  rs::source active_sources_;

  DictionaryInstanceOptions module_config_;
  CameraOptionsType camera_options_;
  SlamState state_;
  std::shared_ptr<rs::slam::occupancy_map> occupancy_map_;

  SlamEventHandlerDev* process_handler_;
  SlamTrackingEventHandlerDev* control_handler_;
};

#endif  // _WORKER_SLAM_MODULE_DEV_H_
