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

class RegisterPersonTaskPayload : public PTAsyncTaskPayload {
 public:
  RegisterPersonTaskPayload(PersonTrackerAdapter* adapter, int32_t id)
      : PTAsyncTaskPayload(adapter),
        id_(id) {}
  virtual ~RegisterPersonTaskPayload() {}
  int32_t id_;
  PersonRegistrationData result_;
};

class RegisterPersonTask : public PTPromiseTask {
 public:
  RegisterPersonTask() {
    task_tag = "RegisterPersonTask";
  }
  virtual ~RegisterPersonTask() {}
  virtual void WorkerThreadExecute();
  virtual RegisterPersonTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class RecognizePersonTaskPayload : public PTAsyncTaskPayload {
 public:
  RecognizePersonTaskPayload(PersonTrackerAdapter* adapter, int32_t id)
      : PTAsyncTaskPayload(adapter),
        id_(id) {}
  virtual ~RecognizePersonTaskPayload() {}
  int32_t id_;
  PersonRecognizerData result_;
};

class RecognizePersonTask : public PTPromiseTask {
 public:
  RecognizePersonTask() {
    task_tag = "RecognizePersonTask";
  }
  virtual ~RecognizePersonTask() {}
  virtual void WorkerThreadExecute();
  virtual RecognizePersonTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class RecognizeAllPersonTaskPayload : public PTAsyncTaskPayload {
 public:
  explicit RecognizeAllPersonTaskPayload(PersonTrackerAdapter* adapter)
      : PTAsyncTaskPayload(adapter) {}
  virtual ~RecognizeAllPersonTaskPayload() {}
  std::vector<PersonRecognizerDataWithStatus*> result_;
};

class RecognizeAllPersonTask : public PTPromiseTask {
 public:
  RecognizeAllPersonTask() {
    task_tag = "RecognizeAllPersonTask";
  }
  virtual ~RecognizeAllPersonTask() {}
  virtual void WorkerThreadExecute();
  virtual RecognizeAllPersonTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class UnRegisterPersonTaskPayload : public PTAsyncTaskPayload {
 public:
  UnRegisterPersonTaskPayload(PersonTrackerAdapter* adapter, int32_t id)
      : PTAsyncTaskPayload(adapter),
        id_(id) {}
  virtual ~UnRegisterPersonTaskPayload() {}
  int32_t id_;
};

class UnRegisterPersonTask : public PTPromiseTask {
 public:
  UnRegisterPersonTask() {
    task_tag = "UnRegisterPersonTask";
  }
  virtual ~UnRegisterPersonTask() {}
  virtual void WorkerThreadExecute();
  virtual UnRegisterPersonTaskPayload* GetPayload();
};

class GetAllRecognitionIDsTaskPayload : public PTAsyncTaskPayload {
 public:
  explicit GetAllRecognitionIDsTaskPayload(PersonTrackerAdapter* adapter)
      : PTAsyncTaskPayload(adapter) {}
  virtual ~GetAllRecognitionIDsTaskPayload() {}
  std::vector<int32_t> result_;
};

class GetAllRecognitionIDsTask : public PTPromiseTask {
 public:
  GetAllRecognitionIDsTask() {
    task_tag = "GetAllRecognitionIDsTask";
  }
  virtual ~GetAllRecognitionIDsTask() {}
  virtual void WorkerThreadExecute();
  virtual GetAllRecognitionIDsTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class RecognitionIDExistTaskPayload : public PTAsyncTaskPayload {
 public:
  RecognitionIDExistTaskPayload(PersonTrackerAdapter* adapter, int32_t id)
      : PTAsyncTaskPayload(adapter),
        id_(id),
        exist_(false) {}
  virtual ~RecognitionIDExistTaskPayload() {}
  int32_t id_;
  bool exist_;
};

class RecognitionIDExistTask : public PTPromiseTask {
 public:
  RecognitionIDExistTask() {
    task_tag = "RecognitionIDExistTask";
  }
  virtual ~RecognitionIDExistTask() {}
  virtual void WorkerThreadExecute();
  virtual RecognitionIDExistTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class GetPersonDescriptorIDsTaskPayload : public PTAsyncTaskPayload {
 public:
  explicit GetPersonDescriptorIDsTaskPayload(PersonTrackerAdapter* adapter,
                                             int32_t recognition_id) :
      PTAsyncTaskPayload(adapter),
      id_(recognition_id) {}
  virtual ~GetPersonDescriptorIDsTaskPayload() {}
  int32_t id_;
  std::vector<int32_t> result_;
};

class GetPersonDescriptorIDsTask : public PTPromiseTask {
 public:
  GetPersonDescriptorIDsTask() {
    task_tag = "GetPersonDescriptorIDsTask";
  }
  virtual ~GetPersonDescriptorIDsTask() {}
  virtual void WorkerThreadExecute();
  virtual GetPersonDescriptorIDsTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
};

class RemovePersonDescriptorTaskPayload : public PTAsyncTaskPayload {
 public:
  explicit RemovePersonDescriptorTaskPayload(PersonTrackerAdapter* adapter,
                                             int32_t recognition_id,
                                             int32_t descriptor_id) :
      PTAsyncTaskPayload(adapter),
      id_(recognition_id),
      descriptor_id_(descriptor_id) {}
  virtual ~RemovePersonDescriptorTaskPayload() {}
  int32_t id_;
  int32_t descriptor_id_;
};

class RemovePersonDescriptorTask : public PTPromiseTask {
 public:
  RemovePersonDescriptorTask() {
    task_tag = "RemovePersonDescriptorTask";
  }
  virtual ~RemovePersonDescriptorTask() {}
  virtual void WorkerThreadExecute();
  virtual RemovePersonDescriptorTaskPayload* GetPayload();
};

#endif  // _WORKER_PERSON_TRACKING_TASKS_H_
