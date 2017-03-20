// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "slam_module.h"

#include <map>
#include <string>

#include "common/camera-options/camera_options_host_instance.h"
#include "common/task/async_task_runner_instance.h"
#include "slam_async_tasks.h"

class SlamCameraDefault : public CameraOptionsDefault {
 public:
  void ResetDefault(CameraOptionsType* target) const override {
    target->has_member_depth = true;
    target->member_depth.has_member_width = true;
    target->member_depth.member_width = 320;
    target->member_depth.has_member_height = true;
    target->member_depth.member_height = 240;
    target->member_depth.has_member_frameRate = true;
    target->member_depth.member_frameRate = 30;
    target->member_depth.has_member_isEnabled = true;
    target->member_depth.member_isEnabled = true;

    target->has_member_fisheye = true;
    target->member_fisheye.has_member_width = true;
    target->member_fisheye.member_width = 640;
    target->member_fisheye.has_member_height = true;
    target->member_fisheye.member_height = 480;
    target->member_fisheye.has_member_frameRate = true;
    target->member_fisheye.member_frameRate = 30;
    target->member_fisheye.has_member_isEnabled = true;
    target->member_fisheye.member_isEnabled = true;
  }
};

// ------------private methods --------------

utils::Status SlamModule::QuerySupportedConfig() {
  utils::Status result;
  utils::ClearStreams(&supported_config_);
  result.set_id(slam_->query_supported_module_config(0, supported_config_));
  if (result.id() < rs::core::status_no_error) {
    result.set_message(
        "failed to query the first supported module configuration");
    return result;
  }

  auto device_name = device_->get_name();
  auto is_current_device_valid =
      (strcmp(device_name, supported_config_.device_name) == 0);
  if (!is_current_device_valid) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(
        "error : current device '"
        + std::string(device_name)
        + "' is not supported by the current supported module configuration");
    return result;
  }
  active_sources_ = utils::GetSourceType(&supported_config_);
  if (!utils::CheckMotionSensorCapabilityIfRequired(
      device_, active_sources_)) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(
        "error : current device is not supported motion events");
  }
  return result;
}

// Check if the module support this option.
bool SlamModule::IsSupported(const CameraOptionsType* src) const {
  const CameraOptionsType& options = *src;
  if (!options.has_member_depth || !options.has_member_fisheye)
    return false;

  const auto& depth_stream = options.member_depth;
  const auto& fisheye_stream = options.member_fisheye;
  if (!fisheye_stream.has_member_width ||
      !fisheye_stream.has_member_height ||
      !fisheye_stream.has_member_frameRate ||
      !fisheye_stream.has_member_isEnabled ||
      !depth_stream.has_member_width ||
      !depth_stream.has_member_height ||
      !depth_stream.has_member_frameRate ||
      !depth_stream.has_member_isEnabled) {
    return false;
  }
  if (!fisheye_stream.member_isEnabled ||
      !depth_stream.member_isEnabled)
    return false;
  rs::core::video_module_interface::supported_module_config config = {};
  for (int i = 0; ; ++i) {
    utils::ClearStreams(&config);
    auto id = slam_->query_supported_module_config(i, config);
    if (id < rs::core::status_no_error) {
      return false;
    }
    auto& fisheye = config[rs::core::stream_type::fisheye];
    auto& depth = config[rs::core::stream_type::depth];
    if (fisheye.size.width == fisheye_stream.member_width &&
        fisheye.size.height == fisheye_stream.member_height &&
        fisheye.frame_rate == fisheye_stream.member_frameRate &&
        depth.size.width == depth_stream.member_width &&
        depth.size.height == depth_stream.member_height &&
        depth.frame_rate == depth_stream.member_frameRate)
      return true;
  }
  return false;
}

std::string SlamModule::GetUnsupportedMessage() const {
  const char* full_msg =
  "You've set CameraOptions to SLAM that are not currently supported." "\n" // NOLINT
  "Try set one of the options in below list:" "\n"
  "-----------------------------------------" "\n"
  "fisheye: 680x480, fps:30 ; depth: 320x240, fps: 30" "\n"
  "-----------------------------------------" "\n";

  return full_msg;
}

void SlamModule::ApplyToCamera(const CameraOptionsType* src) {
  camera_options_.CopyFrom(*src);
}

void SlamModule::PartiallyFillInCameraData(CameraOptionsType* target) {
  camera_options_.IncrementallyCopyFrom(*target);
}

void PostErrorEventTask(std::string* str) {
  auto payload = new ErrorEventPayload(SlamRunner::GetSlamRunner(), str);
  AsyncTaskRunnerInstance::GetInstance()->PostTask(
      new ErrorEventTask(),
      payload,
      "{{EEROR_EVENT MESSAGE}}");
  return;
}

// fill out the actual_config_ and setup the stream callbacks.
utils::Status SlamModule::SetStreamCallbacks() {
  memcpy(
      actual_config_.device_info.name,
      supported_config_.device_name,
      strlen(supported_config_.device_name));

  std::map<rs::core::stream_type, std::function<void(rs::frame)>>
      streamCallbacks;
  for (int i = 0; i < static_cast<int>(rs::core::stream_type::max); ++i) {
    rs::core::stream_type stream = rs::core::stream_type(i);
    auto& supportedStreamConfig = supported_config_[stream];

    if (!supportedStreamConfig.is_enabled)
      continue;

    rs::format streamFormat = rs::format::any;
    if (stream == rs::core::stream_type::depth) {
      streamFormat = rs::format::z16;
    } else if (stream == rs::core::stream_type::fisheye) {
      streamFormat = rs::format::raw8;
    }

    std::shared_ptr<rs::slam::slam> mw = slam_;
    // Which thread will these callbacks run in?
    streamCallbacks[stream] = [stream, mw](rs::frame frame) {
      const auto timestampDomain = frame.get_frame_timestamp_domain();
      if (rs::timestamp_domain::microcontroller != timestampDomain) {
        std::string errorMessage =
            std::string("error: Junk time stamp in stream:")
            + std::to_string(static_cast<int>(stream))
            + std::string("\twith frame counter:")
            + std::to_string(frame.get_frame_number());

        PostErrorEventTask(new std::string(errorMessage));
        return;
      }

      rs::core::correlated_sample_set sampleSet = {};
      sampleSet[stream] =
          rs::core::image_interface::create_instance_from_librealsense_frame(
              frame, rs::core::image_interface::flag::any);

      if (mw->process_sample_set(sampleSet) < rs::core::status_no_error) {
        PostErrorEventTask(new std::string("failed to process sample"));
      }
      sampleSet[stream]->release();
    };
    try {
      rs::stream librealsenseStream = rs::utils::convert_stream_type(stream);
      device_->enable_stream(
          librealsenseStream, supportedStreamConfig.size.width,
          supportedStreamConfig.size.height, streamFormat,
          supportedStreamConfig.frame_rate);

      rs::core::video_module_interface::actual_image_stream_config
          &actualStreamConfig = actual_config_[stream];

      actual_config_[stream].size.width =
          supportedStreamConfig.size.width;

      actual_config_[stream].size.height =
          supportedStreamConfig.size.height;

      actualStreamConfig.frame_rate = supportedStreamConfig.frame_rate;

      actualStreamConfig.intrinsics = rs::utils::convert_intrinsics(
          device_->get_stream_intrinsics(librealsenseStream));
      actualStreamConfig.is_enabled = true;

      device_->set_frame_callback(librealsenseStream, streamCallbacks[stream]);
    } catch(rs::error error) {
      PostErrorEventTask(new std::string(error.what()));
    }
  }  // end of for
  return utils::Status();
}

utils::Status SlamModule::SetMotionCallbacks() {
  // define callback to the motion events and set it, the callback lifetime
  // assumes the module is available.
  std::function<void(rs::motion_data)> motionCallback;
  std::function<void(rs::timestamp_data)> timestampCallback;

  if (utils::IsMotionStreamRequested(active_sources_)) {
    actual_config_[rs::core::motion_type::accel].is_enabled = true;
    actual_config_[rs::core::motion_type::gyro].is_enabled = true;

    std::shared_ptr<rs::slam::slam> mw = slam_;
    motionCallback = [mw](rs::motion_data entry) {
      rs::core::motion_type motion_type =
          utils::RsToSdkMotionType(entry.timestamp_data.source_id);
      if (motion_type != rs::core::motion_type::max) {
        rs::core::correlated_sample_set sampleSet = {};
        sampleSet[motion_type].timestamp = entry.timestamp_data.timestamp;
        sampleSet[motion_type].type = motion_type;
        sampleSet[motion_type].frame_number = entry.timestamp_data.frame_number;
        sampleSet[motion_type].data[0] = entry.axes[0];
        sampleSet[motion_type].data[1] = entry.axes[1];
        sampleSet[motion_type].data[2] = entry.axes[2];

        if (mw->process_sample_set(sampleSet) < rs::core::status_no_error) {
          PostErrorEventTask(new std::string("failed to process sample"));
        }
      }
    };

    timestampCallback = [](rs::timestamp_data entry) {};
    device_->enable_motion_tracking(motionCallback, timestampCallback);

    auto motionIntrinsics = device_->get_motion_intrinsics();
    actual_config_[rs::core::motion_type::accel].intrinsics =
        rs::utils::convert_motion_device_intrinsics(motionIntrinsics.acc);
    actual_config_[rs::core::motion_type::gyro].intrinsics =
        rs::utils::convert_motion_device_intrinsics(motionIntrinsics.gyro);
  }
  actual_config_[rs::core::stream_type::fisheye].extrinsics_motion =
      rs::utils::convert_extrinsics(
          device_->get_motion_extrinsics_from(rs::stream::fisheye));

  actual_config_[rs::core::stream_type::fisheye].extrinsics =
      rs::utils::convert_extrinsics(
          device_->get_extrinsics(rs::stream::depth, rs::stream::fisheye));

  return utils::Status();
}

utils::Status SlamModule::QueryCurrentModuleConfig() {
  utils::Status result;
  float min, max;

  // Query height of interest.
  result.set_id(slam_->get_occupancy_map_height_of_interest(min, max));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to get occupancy map height of interest.");
    module_config_.has_member_occupancyMapHeightOfInterest = false;
    return result;
  }
  module_config_.member_occupancyMapHeightOfInterest.member_min = min;
  module_config_.member_occupancyMapHeightOfInterest.has_member_min = true;
  module_config_.member_occupancyMapHeightOfInterest.member_max = max;
  module_config_.member_occupancyMapHeightOfInterest.has_member_max = true;
  module_config_.has_member_occupancyMapHeightOfInterest = true;

  // Query depth of interest.
  result.set_id(slam_->get_occupancy_map_depth_of_interest(min, max));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to get occupancy map depth of interest.");
    module_config_.has_member_occupancyMapDepthOfInterest = false;
    return result;
  }

  module_config_.member_occupancyMapDepthOfInterest.member_min = min;
  module_config_.member_occupancyMapDepthOfInterest.has_member_min = true;
  module_config_.member_occupancyMapDepthOfInterest.member_max = max;
  module_config_.member_occupancyMapDepthOfInterest.has_member_max = true;
  module_config_.has_member_occupancyMapDepthOfInterest = true;

  // Query if occupancyMapBuilding is enabled automatically.
  module_config_.member_enableOccupancyMapBuilding =
      slam_->is_auto_occupancy_map_building_enabled();
  module_config_.has_member_enableOccupancyMapBuilding = true;

  // Query occupancy_map_resolution.
  module_config_.member_occupancyMapResolution =
      slam_->get_occupancy_map_resolution();
  module_config_.has_member_occupancyMapResolution = true;

  // Query relocalization related flags.
  // There is no way to know from native if relocalization mapping
  // is enabled or not.
  module_config_.has_member_enableRelocalizationMapping = false;
  module_config_.has_member_enableForceRelocalizationPose = false;

  return result;
}

utils::Status SlamModule::ApplyModuleConfig(
    const DictionaryInstanceOptions& external) {
  utils::Status result;

  if (state_ != SlamState::kReady) {
    result.set_id(rs::core::status_init_failed);
    result.set_message("module config need to be set before start");
    return result;
  }

  float min, max;
  if (external.has_member_occupancyMapHeightOfInterest &&
      external.member_occupancyMapHeightOfInterest.has_member_min &&
      external.member_occupancyMapHeightOfInterest.has_member_max) {
    min = external.member_occupancyMapHeightOfInterest.member_min;
    max = external.member_occupancyMapHeightOfInterest.member_max;
    if (module_config_.member_occupancyMapHeightOfInterest.member_min != min ||
        module_config_.member_occupancyMapHeightOfInterest.member_max != max) {
      result.set_id(slam_->set_occupancy_map_height_of_interest(min, max));
      if (result.id() < rs::core::status_no_error) {
        result.set_message("failed to set occupancy map height of interest");
        return result;
      }
      // Set height of interest.
      module_config_.member_occupancyMapHeightOfInterest.member_min = min;
      module_config_.member_occupancyMapHeightOfInterest.has_member_min = true;
      module_config_.member_occupancyMapHeightOfInterest.member_max = max;
      module_config_.member_occupancyMapHeightOfInterest.has_member_max = true;
      module_config_.has_member_occupancyMapHeightOfInterest = true;
    }
  }
  if (external.has_member_occupancyMapDepthOfInterest &&
      external.member_occupancyMapDepthOfInterest.has_member_min &&
      external.member_occupancyMapDepthOfInterest.has_member_max) {
    min = external.member_occupancyMapDepthOfInterest.member_min;
    max = external.member_occupancyMapDepthOfInterest.member_max;
    if (module_config_.member_occupancyMapDepthOfInterest.member_min != min ||
        module_config_.member_occupancyMapDepthOfInterest.member_max != max) {
      result.set_id(slam_->set_occupancy_map_depth_of_interest(min, max));
      if (result.id() < rs::core::status_no_error) {
        result.set_message("failed to set occupancy map depth of interest");
        return result;
      }
      // Set depth of interest.
      module_config_.member_occupancyMapDepthOfInterest.member_min = min;
      module_config_.member_occupancyMapDepthOfInterest.has_member_min = true;
      module_config_.member_occupancyMapDepthOfInterest.member_max = max;
      module_config_.member_occupancyMapDepthOfInterest.has_member_max = true;
      module_config_.has_member_occupancyMapDepthOfInterest = true;
    }
  }

  // Set occupancyMapBuilding.
  if (external.has_member_enableOccupancyMapBuilding) {
    bool enable_map = external.member_enableOccupancyMapBuilding;
    if (module_config_.member_enableOccupancyMapBuilding != enable_map) {
      result.set_id(slam_->set_auto_occupancy_map_building(enable_map));
      if (result.id() < rs::core::status_no_error) {
        result.set_message("failed to set enable occupancy map building");
        return result;
      }
    }
    module_config_.member_enableOccupancyMapBuilding = enable_map;
    module_config_.has_member_enableOccupancyMapBuilding = true;
  }

  // Set occupancy_map_resolution.
  if (external.has_member_occupancyMapResolution) {
    auto resolution = external.member_occupancyMapResolution;
    if (module_config_.member_occupancyMapResolution != resolution) {
      result.set_id(slam_->set_occupancy_map_resolution(resolution));
      if (result.id() < rs::core::status_no_error) {
        result.set_message("failed to set occupancy map resolution");
        return result;
      }
      module_config_.member_occupancyMapResolution = resolution;
      module_config_.has_member_occupancyMapResolution = true;
    }
  }

  // Set relocalization related flags.
  if (external.has_member_enableRelocalizationMapping) {
    auto enable = external.member_enableRelocalizationMapping;
    if (enable) {
      slam_->start_relocalization_mapping();
    } else {
      slam_->stop_relocalization_mapping();
    }
    module_config_.member_enableRelocalizationMapping = enable;
    module_config_.has_member_enableRelocalizationMapping = true;
  }

  if (external.has_member_enableForceRelocalizationPose) {
    auto enable = external.member_enableForceRelocalizationPose;
    slam_->force_relocalization_pose(enable);
    module_config_.member_enableForceRelocalizationPose = enable;
    module_config_.has_member_enableForceRelocalizationPose = true;
  }
  return result;
}

// ------------public methods --------------

SlamState SlamModule::state() {
  return state_;
}

utils::Status SlamModule::ExportModuleConfig(
    DictionaryInstanceOptions* module_config) {
  utils::Status result;

  if (state_ <= SlamState::kIdle) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(
        "wrong state: try to get module config before initialization.");
    return result;
  }
  module_config->CopyFrom(module_config_);
  return result;
}

// ------------controllers --------------
utils::Status SlamModule::Init() {
  utils::Status result;

  // 0. check the state.
  if (state_ != SlamState::kIdle) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(
        "wrong state: try to init the module in a wrong state.");
    return result;
  }

  // 0. Create a slam object.
  slam_ = std::make_shared<rs::slam::slam>();

  // 1. Get device ready.
  if (rs_context_.get_device_count() == 0) {
    result.set_id(rs::core::status_device_lost);
    return result;
  }

  device_ = rs_context_.get_device(0);

  // 2. Query supported config.
  result = QuerySupportedConfig();
  if (result.id() < rs::core::status_no_error) return result;

  // 3. construct the actual config object which will be applied.
  //    and set stream&motion callbacks
  result = SetStreamCallbacks();
  if (result.id() < rs::core::status_no_error) return result;

  result = SetMotionCallbacks();
  if (result.id() < rs::core::status_no_error) return result;

  // 4. apply camera config
  // set_module_config actually apply configurations for streams and motion,
  // so it applys to the camera_config_.
  // TODO(Donna): we need to change module_config_ to a better name, such as
  // instance_options_, etc.
  // It's better to move following lines to start function, but there is a
  // native caused bug(#98) that set_module_config can not been called on one
  // slam object more than once. So we must create a new slam object if we need
  // to reset the camera config. The good news is SLAM module supports only a
  // signle configuration currently.
  result.set_id(slam_->set_module_config(actual_config_));
  if (result.id() < rs::core::status_no_error) {
    result.set_message(
        "error : failed to set the enabled configuration");
    return result;
  }

  // 5. get default module config
  QueryCurrentModuleConfig();

  // 6. register MW event handlers
  process_handler_ = new SlamEventHandler();
  result.set_id(slam_->register_event_handler(process_handler_));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to set slamEventHandler");
    return result;
  }

  control_handler_ = new SlamTrackingEventHandler();
  result.set_id(slam_->register_tracking_event_handler(control_handler_));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to set slamTrackingEventHandler");
    return result;
  }

  // 7. change the running state.
  state_ = SlamState::kReady;
  return result;
}

void SlamModule::RegisterToCameraHost() {
  CameraOptionsHost* options_host = CameraOptionsHostInstance::GetInstance();

  // Make SLAM default options merged with the generals.
  options_host->SetCameraOptionsDefault(new SlamCameraDefault());
  options_host->ResetDefaultCameraOptions();

  // Register to the camera host.
  options_host->GetCameraOptionsIO()->Add(this);
}

void SlamModule::CleanUp() {
  state_ = SlamState::kIdle;
  slam_.reset();
  occupancy_map_.reset();
  if (process_handler_) {
    delete process_handler_;
    process_handler_ = nullptr;
  }

  if (control_handler_) {
    delete control_handler_;
    process_handler_ = nullptr;
  }
}

utils::Status SlamModule::Reset() {
  utils::Status result;

  // Device has been started.
  if (state_ >= SlamState::kTracking) {
    result = StopDevice();
    if (result.id() < rs::core::status_no_error) return result;
  }

  CleanUp();
  return Init();
}

utils::Status SlamModule::Start() {
  utils::Status result;

  // 1. check if the module had been initialized.
  if (state_ != SlamState::kReady) {
    result.set_id(SLAM_ADDON_ERROR);
    std::string msg = std::string((state_ < SlamState::kReady) ?
        "try to start without initialization" : "already started");
    result.set_message("wrong state:" + msg);
    return result;
  }
  // 2. start the device
  try {
    // enable fisheye strobe
    device_->set_option(rs::option::fisheye_strobe, 1);
    device_->set_option(rs::option::r200_lr_auto_exposure_enabled, 1);
    // Enable auto exposure for fisheye camera stream
    device_->set_option(rs::option::fisheye_color_auto_exposure, 1);
    device_->start(active_sources_);
  } catch (rs::error e) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(std::string("SlamModule::Start, ") + e.what());
    return result;
  }

  state_ = SlamState::kTracking;
  return result;
}

OutputHolder* SlamModule::MoveTrackingResult() {
  return process_handler_->MoveResult();
}

utils::Status SlamModule::GetOccupancyMapUpdate(
    ParameterWrapperForOccupancyMap* args) {
  utils::Status result;
  if (args == nullptr || args->map_data == nullptr) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message("Null data storage to get occupancy map update.");
    return result;
  }
  if (!occupancy_map_) {
    occupancy_map_ =
        slam_->create_occupancy_map(DEFAULT_OCCUPANCY_MAP_CAPACITY);
  }

  if (args->has_roi) {
    rs::core::pointI32 min_point, max_point;
    min_point.x = args->roi.member_minX;
    min_point.y = args->roi.member_minY;
    max_point.x = args->roi.member_maxX;
    max_point.y = args->roi.member_maxY;
    result.set_id(slam_->get_occupancy_map_update(
        occupancy_map_, min_point, max_point));
  } else {
    result.set_id(slam_->get_occupancy_map_update(occupancy_map_));
  }
  if (result.id() < rs::core::status_no_error) {
    result.set_message("Failed to get occpuancy map update.");
    return result;
  }

  args->map_data->SetData(result.id() == rs::core::status_data_pending,
      occupancy_map_->get_tile_count(), occupancy_map_->get_tile_coordinates());
  return result;
}

utils::Status SlamModule::GetOccupancyMap(
    ParameterWrapperForOccupancyMap* args) {
  utils::Status result;
  if (args == nullptr || args->map_data == nullptr) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message("Null data storage to get occupancy map.");
    return result;
  }
  if (!occupancy_map_) {
    occupancy_map_ =
        slam_->create_occupancy_map(DEFAULT_OCCUPANCY_MAP_CAPACITY);
  }

  if (args->has_roi) {
    rs::core::pointI32 min_point, max_point;
    min_point.x = args->roi.member_minX;
    min_point.y = args->roi.member_minY;
    max_point.x = args->roi.member_maxX;
    max_point.y = args->roi.member_maxY;
    result.set_id(
        slam_->get_occupancy_map(occupancy_map_, min_point, max_point));
  } else {
    result.set_id(slam_->get_occupancy_map(occupancy_map_));
  }
  if (result.id() < rs::core::status_no_error) {
    result.set_message("Failed to get occpuancy map.");
    return result;
  }

  args->map_data->SetData(result.id() == rs::core::status_data_pending,
      occupancy_map_->get_tile_count(), occupancy_map_->get_tile_coordinates());
  return result;
}

utils::Status SlamModule::GetOccupancyMapAsRgba(
    MapImage* map_image,
    bool draw_pose_trajectory,
    bool draw_occupancy_map) {
  utils::Status result;
  unsigned char *image;
  unsigned int width, height;
  width = 0;
  height = 0;
  result.set_id(slam_->get_occupancy_map_as_rgba(&image, &width, &height,
      draw_pose_trajectory, draw_occupancy_map));

  if (result.id() < rs::core::status_no_error) {
    result.set_message("Failed to get occpuancy map as rgba.");
    return result;
  }
  if (image == nullptr || width < 0 || height < 0) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message("Invalid RGBA occupancy map data.");
    return result;
  }
  map_image->set_data_store(image, width, height);
  map_image->set_width(width);
  map_image->set_height(height);
  return result;
}

utils::Status SlamModule::GetOccupancyMapBounds(
    OccupancyMapBounds* map_bounds) {
  utils::Status result;
  if (map_bounds == nullptr) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message("Null data storage to get map bounds.");
    return result;
  }

  int32_t min_x, min_z, max_x, max_z;
  result.set_id(slam_->get_occupancy_map_bounds(min_x, min_z, max_x, max_z));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("Failed to get occpuancy map bounds.");
    return result;
  }

  map_bounds->set_minX(min_x);
  map_bounds->set_minZ(min_z);
  map_bounds->set_maxX(max_x);
  map_bounds->set_maxZ(max_z);
  return result;
}

utils::Status SlamModule::SaveOccupancyMapAsPpm(
    const std::string& file_name, bool draw_camera_trajectory) {
  utils::Status result;
  result.set_id(
      slam_->save_occupancy_map_as_ppm(file_name, draw_camera_trajectory));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to save occpuancy map.");
  }

  return result;
}

utils::Status SlamModule::SaveOccupancyMap(const std::string& file_name) {
  utils::Status result;
  result.set_id(slam_->save_occupancy_map(file_name));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to save occpuancy map.");
  }

  return result;
}

utils::Status SlamModule::LoadOccupancyMap(const std::string& file_name) {
  utils::Status result;
  result.set_id(slam_->load_occupancy_map(file_name));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to load relocalization map.");
  }
  return result;
}

utils::Status SlamModule::SaveRelocalizationMap(
    const std::string& file_name) {
  utils::Status result;
  result.set_id(slam_->save_relocalization_map(file_name));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to save occpuancy map.");
  }
  return result;
}

utils::Status SlamModule::LoadRelocalizationMap(
    const std::string& file_name) {
  utils::Status result;
  result.set_id(slam_->load_relocalization_map(file_name));
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to load relocalization map.");
  }
  return result;
}

utils::Status SlamModule::GetRelocalizationPose(float* relocalization_pose) {
  utils::Status result;
  rs::slam::PoseMatrix4f pose;
  result.set_id(slam_->get_relocalization_pose(pose));

  for (int i = 0; i < 16; i++) {
    relocalization_pose[i] = pose.m_data[i];
  }
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to get relocalization pose.");
  }
  return result;
}

utils::Status SlamModule::RestartTracking() {
  utils::Status result;

  // check if the module had been initialized.
  if (state_ != SlamState::kTracking) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(
        "wrong state to restart slam module, not tracking");
    return result;
  }

  result.set_id(slam_->restart());
  if (result.id() < rs::core::status_no_error) {
    result.set_message("failed to restart slam module");
  }

  return result;
}

// TODO(Donna): implement reset, check the detination state.
// utils::Status reset() {}

utils::Status SlamModule::Stop() {
  utils::Status result;

  // check if the module had been initialized.
  if (state_ == SlamState::kIdle) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message("wrong state to stop slam module, not initialized");
    return result;
  }

  result = StopDevice();
  if (result.id() < rs::core::status_no_error) return result;

  state_ = SlamState::kReady;
  return result;
}

utils::Status SlamModule::StopDevice() {
  utils::Status result;
  try {
    slam_->flush_resources();
    device_->stop(active_sources_);
  } catch (rs::error e) {
    result.set_id(SLAM_ADDON_ERROR);
    result.set_message(std::string("SLAM StopDevice, ") + e.what());
    return result;
  }
  return result;
}
