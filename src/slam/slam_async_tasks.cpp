// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
#include "slam_async_tasks.h"

#include <string>

#include "gen/nan__event_info.h"
#include "gen/nan__occupancy_map_data.h"
#include "gen/nan__tracking_result.h"
#include "gen/array_helper.h"
#include "common/camera-options/camera_options_host_instance.h"

/////////////////////////////////////////////////////////////////////////////
GetInstanceOptionsTask::GetInstanceOptionsTask() {
  task_tag = "getInstanceOptions() task";
}

GetInstanceOptionsTask::~GetInstanceOptionsTask() {
}

void GetInstanceOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  result_status = payload->GetSlamModule()->ExportModuleConfig(payload->data());
  task_state =
      (result_status.id() >= rs::core::status_no_error) ? Successful : Failed;
}

v8::Local<v8::Value> GetInstanceOptionsTask::GetResolved() {
  InstanceOptions dictionary;
  auto data = GetPayload()->data();
  data->ExportTo(dictionary);
  // All data has been copied to InstanceOptions, so delete this pointer.
  delete data;

  return static_cast<v8::Local<v8::Object>>(dictionary);
}

InstanceOptionsTaskPayload* GetInstanceOptionsTask::GetPayload() {
  return reinterpret_cast<InstanceOptionsTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
SetInstanceOptionsTask::SetInstanceOptionsTask() {
  task_tag = "setInstanceOptions() task";
}

SetInstanceOptionsTask::~SetInstanceOptionsTask() {
}

void SetInstanceOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  auto data = payload->data();
  result_status =
      payload->GetSlamModule()->ApplyModuleConfig(*data);
  delete data;

  task_state =
      (result_status.id() >= rs::core::status_no_error) ? Successful : Failed;
}

InstanceOptionsTaskPayload* SetInstanceOptionsTask::GetPayload() {
  return reinterpret_cast<InstanceOptionsTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
StartTask::StartTask() {
  task_tag = "start() task";
}

StartTask::~StartTask() {
}

void StartTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  result_status = payload->GetSlamModule()->Start();
  task_state =
      (result_status.id() >= rs::core::status_no_error) ? Successful : Failed;
}

SlamPayload<void>* StartTask::GetPayload() {
  return reinterpret_cast<SlamPayload<void>*>(AsyncTask::GetPayload());
}
/////////////////////////////////////////////////////////////////////////////
StopTask::StopTask() {
  task_tag = "stop() task";
}

StopTask::~StopTask() {
}

void StopTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  result_status = payload->GetSlamModule()->Stop();
  task_state =
      (result_status.id() >= rs::core::status_no_error) ? Successful : Failed;
}

SlamPayload<void>* StopTask::GetPayload() {
  return reinterpret_cast<SlamPayload<void>*>(AsyncTask::GetPayload());
}
/////////////////////////////////////////////////////////////////////////////
v8_value_t TrackingEventTask::PopEventData(size_t event_index) {
  auto data = GetPayload()->data();
  auto eventData = data->ToEventInfo();
  // eventData will deep copy the data, so release the data.
  delete data;

  return NanEventInfo::NewInstance(eventData);
}
/////////////////////////////////////////////////////////////////////////////
GetTrackingResultTask::GetTrackingResultTask() {
  task_tag = "getTrackingResult() task";
}

GetTrackingResultTask::~GetTrackingResultTask() {
}

void GetTrackingResultTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  payload->set_data(payload->GetSlamModule()->MoveTrackingResult());
  task_state = Successful;
}

v8::Local<v8::Value> GetTrackingResultTask::GetResolved() {
  auto data = GetPayload()->data();
  if (data == nullptr) {
    return Nan::Undefined();
  }
  auto output = data->ToTrackingResult();
  delete data;
  if (output == nullptr) return Nan::Undefined();

  return NanTrackingResult::NewInstance(output);
}

TrackingResultPayload* GetTrackingResultTask::GetPayload() {
  return reinterpret_cast<TrackingResultPayload*>(AsyncTask::GetPayload());
}
/////////////////////////////////////////////////////////////////////////////
GetOccupancyMapUpdateTask::GetOccupancyMapUpdateTask() {
  task_tag = "getOccupancyMapUpdate() task";
}

GetOccupancyMapUpdateTask::~GetOccupancyMapUpdateTask() {
}

void GetOccupancyMapUpdateTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  result_status = payload->GetSlamModule()->GetOccupancyMapUpdate(
      payload->data());
  task_state =
      (result_status.id() >= rs::core::status_no_error) ? Successful : Failed;
}

v8::Local<v8::Value> GetOccupancyMapUpdateTask::GetResolved() {
  auto map_data = GetPayload()->data();
  map_data->SetupTypedArray();
  return NanOccupancyMapData::NewInstance(map_data);
}

SlamPayload<OccupancyMapData*>* GetOccupancyMapUpdateTask::GetPayload() {
  return reinterpret_cast<SlamPayload<OccupancyMapData*>*>(
      AsyncTask::GetPayload());
}
/////////////////////////////////////////////////////////////////////////////
GetCameraOptionsTask::GetCameraOptionsTask() {
  task_tag = "getCameraOptions() task";
}

GetCameraOptionsTask::~GetCameraOptionsTask() {
}

void GetCameraOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  CameraOptionsHost* options_host = CameraOptionsHostInstance::GetInstance();
  auto& options = options_host->GetCameraOptions();
  auto data = payload->data();
  delete data;
  payload->set_data(&(options));
  task_state = Successful;
}

v8::Local<v8::Value> GetCameraOptionsTask::GetResolved() {
  CameraOptions dictionary;
  auto data = GetPayload()->data();
  if (data) {
    data->ExportTo(dictionary);
    return static_cast<v8::Local<v8::Object>>(dictionary);
  }
  return Nan::Undefined();
}

CameraOptionsTaskPayload* GetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
SetCameraOptionsTask::SetCameraOptionsTask() {
  task_tag = "setCameraOptions() task";
}

SetCameraOptionsTask::~SetCameraOptionsTask() {
}

void SetCameraOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  auto options = payload->data();

  // Apply options
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  CameraOptionsType previous = host->GetCameraOptions();
  host->PartiallyOverwriteCameraOptions(*options);

  try {
    if (host->Validate()) {
      host->WriteToCamera();
      task_state = Successful;
    } else {
      auto message = host->GetValidationFailureMessage();
      throw message;
    }
  } catch (const std::string& message) {
    task_state = Failed;
    host->SetCameraOptions(previous);
    result_status.set_id(SLAM_ADDON_ERROR);
    result_status.set_message(message);
  } catch (...) {
    task_state = Failed;
    result_status.set_id(SLAM_ADDON_ERROR);
    result_status.set_message("Failed to write to camera.");
    host->SetCameraOptions(previous);
  }
}

CameraOptionsTaskPayload* SetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}
