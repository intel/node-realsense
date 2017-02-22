// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_ALL_REQUESTS_H_
#define _WORKER_ALL_REQUESTS_H_

#include <memory>
#include <string>

#include "common/camera-options/camera_options_host_instance.h"
#include "common/camera-options/camera_options_type.h"
#include "common/task/async_task.h"
#include "gen/camera_options.h"
#include "gen/instance_options.h"
#include "slam_runner_dev.h"
#include "slam_task_types.h"

using InstanceOptionsTaskPayload = SlamPayload<DictionaryInstanceOptions*>;
using TrackingEventPayload = SlamPayload<TrackedResultInternal*>;
using TrackingResultPayload = SlamPayload<OutputHolder*>;
using CameraOptionsTaskPayload = SlamPayload<DictionaryCameraOptions*>;

//
// Request for: getInstanceOptions
//
class GetInstanceOptionsTask : public SlamPromiseTask {
 public:
  GetInstanceOptionsTask();
  ~GetInstanceOptionsTask();
  void WorkerThreadExecute() override;
  v8::Local<v8::Value> GetResolved() override;
  InstanceOptionsTaskPayload* GetPayload();
};

//
// Request for: setInstanceOptions
//
class SetInstanceOptionsTask : public SlamPromiseTask {
 public:
  SetInstanceOptionsTask();
  ~SetInstanceOptionsTask();

  void WorkerThreadExecute() override;
  // We don't need to override MainThreadDispatchResult(),
  // default behavior is good enough
  InstanceOptionsTaskPayload* GetPayload();
};

//
// Request for: start
//
class StartTask : public SlamPromiseTask {
 public:
  StartTask();
  ~StartTask();

  void WorkerThreadExecute() override;
  SlamPayload<void>* GetPayload();
};

//
// Request for: start
//
class StopTask : public SlamPromiseTask {
 public:
  StopTask();
  ~StopTask();

  void WorkerThreadExecute() override;
  SlamPayload<void>* GetPayload();
};

//
// Request for: tracking event
//
// TDDO(Donna) use template for all event tasks, SlamEventTask<T>, T names
// the
class TrackingEventTask : public SlamEventTask {
 public:
  TrackingEventTask() : SlamEventTask(SlamEvent::kTracking) {}

  v8_value_t PopEventData(size_t event_index) override;

  TrackingEventPayload* GetPayload() {
    return reinterpret_cast<TrackingEventPayload*>(AsyncTask::GetPayload());
  }
};

//
// Request for: getTrackingResult
//
class GetTrackingResultTask : public SlamPromiseTask {
 public:
  GetTrackingResultTask();
  ~GetTrackingResultTask();

  void WorkerThreadExecute() override;
  TrackingResultPayload* GetPayload();
  v8::Local<v8::Value> GetResolved() override;
};

//
// Request for: getOccupancyMapUpdateTask
//
class GetOccupancyMapUpdateTask : public SlamPromiseTask {
 public:
  GetOccupancyMapUpdateTask();
  ~GetOccupancyMapUpdateTask();

  void WorkerThreadExecute() override;
  SlamPayload<OccupancyMapData*>* GetPayload();
  v8::Local<v8::Value> GetResolved() override;
};

//
// Request for: getCameraOptions
//
class GetCameraOptionsTask : public SlamPromiseTask {
 public:
  GetCameraOptionsTask();
  ~GetCameraOptionsTask();

  void WorkerThreadExecute() override;
  v8::Local<v8::Value> GetResolved() override;
  CameraOptionsTaskPayload* GetPayload();
};

//
// Request for: setCameraOptions
//
class SetCameraOptionsTask : public SlamPromiseTask {
 public:
  SetCameraOptionsTask();
  ~SetCameraOptionsTask();
  void WorkerThreadExecute() override;
  CameraOptionsTaskPayload* GetPayload();
};

#endif  // _WORKER_ALL_REQUESTS_H_
