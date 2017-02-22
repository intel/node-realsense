// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <string>

#include "argument_wrapper.h"
#include "map_image.h"
#include "worker/all_requests.h"

#include "gen/nan__instance.h"
#include "gen/nan__tracking_result.h"
#include "gen/nan__occupancy_map_data.h"
#include "gen/nan__occupancy_map_bounds.h"
#include "gen/nan__map_image.h"

//
// Request for createInstance
//
void ReqCreateInstance::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  result_status_ = runner_->slam_module()->Init();
}

void ReqCreateInstance::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  Instance* i = static_cast<Instance*>(argument_);

  // If the slam failed to init, stop the worker and return undefined.
  if (result_status_.id() < rs::core::status_no_error) {
    runner_->StopWorker();
    promise_helper_.RejectPromise(result_status_.message());
  } else {
    // Else return a Instance Nan object.
    promise_helper_.ResolvePromise(NanInstance::NewInstance(i, true));
  }
  v8::Isolate::GetCurrent()->RunMicrotasks();
}

//
// Request for getInstanceOptions.
//
void ReqGetInstanceOptions::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (result_data_ != nullptr) delete result_data_;

  result_data_ = new DictionaryInstanceOptions(
      runner_->slam_module()->module_config());
  result_status_ = utils::Status(rs::core::status_no_error);
}

void ReqGetInstanceOptions::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  DictionaryInstanceOptions* module_config =
      static_cast<DictionaryInstanceOptions*>(result_data_);

  ResolvePromise(module_config->Exported().GetObject());
}

//
// Request for setInstanceOptions.
//
void ReqSetInstanceOptions::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (argument_ == nullptr) {
    result_status_ = utils::Status(rs::core::status_param_unsupported,
        "null parameter to setInstanceOptions");
    return;
  }
  // A deep copy of ModuleConfig.
  DictionaryInstanceOptions module_config =
      *(static_cast<DictionaryInstanceOptions*>(argument_));
  result_status_ = runner_->slam_module()->ApplyModuleConfig(module_config);
}

void ReqSetInstanceOptions::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  ResolvePromise();
}

//
// Request for getOccupancyMapUpdate
//
void ReqGetOccupancyMapUpdate::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_data = new OccupancyMapData();
  result_status_ = runner_->slam_module()->GetOccupancyMapUpdate(map_data);
  result_data_ = map_data;
}

void ReqGetOccupancyMapUpdate::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_data = static_cast<OccupancyMapData*>(result_data_);
  map_data->SetupTypedArray();
  ResolvePromise(NanOccupancyMapData::NewInstance(map_data));
}

//
// Request for: getOccupancyMapAsRgba
//
void ReqGetOccupancyMapAsRgba::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_image = new MapImage();
  ArgumentUnit argument_unit = *(static_cast<ArgumentUnit*>(argument_));
  bool draw_pose_trajectory = argument_unit.draw_pose_trajectory;
  bool draw_occupancy_map = argument_unit.draw_occupancy_map;

  result_status_ = runner_->slam_module()->GetOccupancyMapAsRgba(
      map_image, draw_pose_trajectory, draw_occupancy_map);
  result_data_ = map_image;
}

void ReqGetOccupancyMapAsRgba::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_image = static_cast<MapImage*>(result_data_);
  map_image->SetupTypedArray();
  ResolvePromise(NanMapImage::NewInstance(map_image));
}

//
// Request for: getOccupancyMapBounds
//
void ReqGetOccupancyMapBounds::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_bounds = new OccupancyMapBounds();
  result_status_ = runner_->slam_module()->GetOccupancyMapBounds(map_bounds);
  result_data_ = map_bounds;
}

void ReqGetOccupancyMapBounds::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  auto map_bounds = static_cast<OccupancyMapBounds*>(result_data_);
  ResolvePromise(NanOccupancyMapBounds::NewInstance(map_bounds));
}

//
// Request for saveOccpuancyMapAsPpm
//
void ReqSaveOccupancyMapAsPpm::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (argument_ == nullptr) {
    result_status_ = utils::Status(rs::core::status_param_unsupported,
        "null parameter to saveOccpuancyMapAsPpm");
    return;
  }
  ArgumentWrapper map_ppm = *(static_cast<ArgumentWrapper*>(argument_));
  std::string file_name = map_ppm.file_name;
  bool draw_camera_trajectory = map_ppm.draw_camera_trajectory;

  result_status_ = runner_->slam_module()->SaveOccupancyMapAsPpm(
      file_name, draw_camera_trajectory);

  result_data_ = new std::string(file_name);
}

void ReqSaveOccupancyMapAsPpm::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  std::string file_name = *(static_cast<std::string*>(result_data_));
  ResolvePromise(Nan::New(file_name.c_str()).ToLocalChecked());
}

//
// Request for saveOccpuancyMap
//
void ReqSaveOccupancyMap::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (argument_ == nullptr) {
    result_status_ = utils::Status(rs::core::status_param_unsupported,
        "null parameter to saveOccpuancyMap");
    return;
  }
  std::string file_name = *(static_cast<std::string*>(argument_));

  utils::Status result_status_ = runner_->slam_module()->SaveOccupancyMap(
      file_name);

  result_data_ = new std::string(file_name);
}

void ReqSaveOccupancyMap::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  std::string file_name = *(static_cast<std::string*>(result_data_));
  ResolvePromise(Nan::New(file_name.c_str()).ToLocalChecked());
}

//
// Request for getTrackingResult.
//
void ReqGetTrackingResult::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  result_data_ = runner_->slam_module()->MoveTrackingResult();
}

void ReqGetTrackingResult::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (!result_data_) {
    ResolvePromise(Nan::Undefined());
    return;
  }
  auto output = static_cast<OutputHolder*>(result_data_)->ToTrackingResult();

  ResolvePromise(NanTrackingResult::NewInstance(output));
}

//
// Request for start.
//
void ReqStart::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  result_status_ = runner_->slam_module()->Start();
}

void ReqStart::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  ResolvePromise();
}

//
// Request for stop.
//
void ReqStop::DoWork() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  result_status_ = runner_->slam_module()->Stop();
}

void ReqStop::Resolve() {
  std::lock_guard<std::mutex> lock(request_mutex_);
  runner_->StopWorker();
  ResolvePromise();
}
