// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_TRACKING_TASKS_H_
#define _WORKER_PERSON_TRACKING_TASKS_H_

#include <v8.h>

#include <memory>
#include <string>
#include <vector>

#include "common/task/async_task.h"
#include "gen/promise-helper.h"
#include "worker/person_tracker_adapter.h"

class PTAsyncTaskPayload : public AsyncTaskPayload {
 public:
  explicit PTAsyncTaskPayload(PersonTrackerAdapter* adapter)
      : AsyncTaskPayload(), adapter_(adapter) {}
  virtual ~PTAsyncTaskPayload() {}
  PersonTrackerAdapter* GetAdapter() { return adapter_; }
 protected:
  PersonTrackerAdapter* adapter_;
};

class PTPromiseTask : public PromiseTask {
 public:
  PTPromiseTask() : reject_reason_("Failed") {}
  virtual ~PTPromiseTask() {}
  virtual PTAsyncTaskPayload* GetPayload();
  virtual PersonTrackerAdapter* GetAdapter();
  virtual v8_value_t GetRejected();

 protected:
  // The reject_reason_ shall be set to meaningful value.
  std::string reject_reason_;
};

class PTEventEmitterTask : public EventEmitterTask {
 public:
  PTEventEmitterTask() {}
  virtual ~PTEventEmitterTask() {}
  virtual PTAsyncTaskPayload* GetPayload();
  virtual PersonTrackerAdapter* GetAdapter();
};

class SetOptionsTaskPayload : public PTAsyncTaskPayload {
  SetOptionsTaskPayload(const SetOptionsTaskPayload&) = delete;
  SetOptionsTaskPayload& operator=(const SetOptionsTaskPayload&) = delete;

 public:
  SetOptionsTaskPayload(
      std::shared_ptr<DictionaryPersonTrackerOptions> options,
      PersonTrackerAdapter* adapter)
      : PTAsyncTaskPayload(adapter) {
    options_ = options;
  }
  virtual ~SetOptionsTaskPayload() {}
  std::shared_ptr<DictionaryPersonTrackerOptions> GetOptions() {
    return options_;
  }

 private:
  std::shared_ptr<DictionaryPersonTrackerOptions> options_;
};

class SetOptionsTask : public PTPromiseTask {
 public:
  SetOptionsTask() { task_tag = "SetOptionsTask";}
  virtual ~SetOptionsTask() {}
  virtual void WorkerThreadExecute();
  virtual SetOptionsTaskPayload* GetPayload();
};

class GetOptionsTask : public PTPromiseTask {
 public:
  GetOptionsTask() { task_tag = "GetOptionsTask";}
  virtual ~GetOptionsTask() {}
  virtual void WorkerThreadExecute();
  virtual v8_value_t GetResolved();
};

class CameraOptionsTaskPayload : public PTAsyncTaskPayload {
  CameraOptionsTaskPayload(const CameraOptionsTaskPayload&) = delete;
  CameraOptionsTaskPayload& operator=(
      const CameraOptionsTaskPayload&) = delete;

 public:
  CameraOptionsTaskPayload(
      PersonTrackerAdapter* adapter,
      std::shared_ptr<DictionaryCameraOptions> options = nullptr)
      : PTAsyncTaskPayload(adapter),
        options_(options) {}
  virtual ~CameraOptionsTaskPayload() {}
  std::shared_ptr<DictionaryCameraOptions> GetOptions() {
      return options_;
  }

  std::shared_ptr<DictionaryCameraOptions> options_;
};

class SetCameraOptionsTask : public PTPromiseTask {
 public:
  SetCameraOptionsTask() { task_tag = "SetCameraOptionsTask";}
  virtual ~SetCameraOptionsTask() {}
  virtual void WorkerThreadExecute();
  virtual CameraOptionsTaskPayload* GetPayload();
};

class GetCameraOptionsTask : public PTPromiseTask {
 public:
  GetCameraOptionsTask() { task_tag = "GetCameraOptionsTask";}
  virtual ~GetCameraOptionsTask() {}
  virtual void WorkerThreadExecute();
  virtual v8_value_t GetResolved();
  virtual CameraOptionsTaskPayload* GetPayload();
};

class StartPersonTrackingTask : public PTPromiseTask {
 public:
  StartPersonTrackingTask();
  virtual ~StartPersonTrackingTask();
  virtual void WorkerThreadExecute();
};

class StartOrStopTrackingOnePersonTaskPayload : public PTAsyncTaskPayload {
  StartOrStopTrackingOnePersonTaskPayload(
      const StartOrStopTrackingOnePersonTaskPayload& ) = delete;
  StartOrStopTrackingOnePersonTaskPayload& operator=(
      const StartOrStopTrackingOnePersonTaskPayload&) = delete;

 public:
  StartOrStopTrackingOnePersonTaskPayload(
      PersonTrackerAdapter* adapter,
      int32_t track_id,
      bool is_start)
          : PTAsyncTaskPayload(adapter),
            id_(track_id),
            is_start_(is_start) {}
  virtual ~StartOrStopTrackingOnePersonTaskPayload() {}
  int32_t id_;
  bool is_start_;
};

class StartOrStopTrackingOnePersonTask : public PTPromiseTask {
 public:
  StartOrStopTrackingOnePersonTask() {
    task_tag = "StartOrStopTrackingOnePersonTask";
  }
  virtual ~StartOrStopTrackingOnePersonTask() {}
  virtual void WorkerThreadExecute();
  virtual StartOrStopTrackingOnePersonTaskPayload* GetPayload();
};

class RunPersonTrackingTask : public PTEventEmitterTask {
 public:
  RunPersonTrackingTask()
      : request_cancel_(false),
        request_pause_(false),
        request_resume_(false),
        error_happen_(false) {
    task_tag = "RunPersonTrackingTask";
  }
  virtual ~RunPersonTrackingTask() {}
  virtual void ReloadIfNeeded();
  virtual void WorkerThreadExecute();
  virtual void MainThreadDispatchResult();
  virtual bool ShouldPopEvent(size_t event_index);
  virtual v8_value_t PopEventData(size_t event_index);
  virtual std::string GetEventName(size_t event_index) const;
  virtual size_t GetEventCount();
  virtual void RequestCancel();
  virtual void RequestPause();
  virtual void RequestResume();
  bool request_cancel_;
  bool request_pause_;
  bool request_resume_;
  bool error_happen_;
};

class ResetTask : public PTPromiseTask {
 public:
  ResetTask() { task_tag = "ResetTask";}
  virtual ~ResetTask() {}
  virtual void WorkerThreadExecute();
};

class GetPersonInfoTaskPayload : public PTAsyncTaskPayload {
  GetPersonInfoTaskPayload(const GetPersonInfoTaskPayload&) = delete;
  GetPersonInfoTaskPayload& operator=(
      const GetPersonInfoTaskPayload&) = delete;

 public:
  GetPersonInfoTaskPayload(PersonTrackerAdapter* adapter, int32_t track_id)
      : PTAsyncTaskPayload(adapter),
        id_(track_id) {}
  virtual ~GetPersonInfoTaskPayload() {}
  int32_t id_;
  std::shared_ptr<PersonDataInternal> person_data_;
};

class GetPersonInfoTask : public PTPromiseTask {
 public:
  GetPersonInfoTask() {
    task_tag = "GetPersonInfoTask";
  }
  virtual ~GetPersonInfoTask() {}
  virtual void WorkerThreadExecute();
  virtual v8_value_t GetResolved();
  virtual GetPersonInfoTaskPayload* GetPayload();
};

#endif  // _WORKER_PERSON_TRACKING_TASKS_H_
