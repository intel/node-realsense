// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OBJECT_RECOGNITION_TASKS_H_
#define _OBJECT_RECOGNITION_TASKS_H_

#include <memory>
#include <string>
#include <vector>

#include "common/task/async_task.h"
#include "gen/camera_options.h"
#include "gen/object_recognition_options.h"
#include "object_recognition_runner.h"

/////////////////////////////////////////////////////////////////////////////
class InstanceOptionsTaskPayload : public AsyncTaskPayload {
  InstanceOptionsTaskPayload(const InstanceOptionsTaskPayload&) = delete;
  InstanceOptionsTaskPayload& operator = (const InstanceOptionsTaskPayload&)
      = delete;

 public:
  InstanceOptionsTaskPayload(
      std::shared_ptr<DictionaryObjectRecognitionOptions> options,
      ObjectRecognitionRunner* runner) {
    options_ = options;
    runner_ = runner;
  }

  DictionaryObjectRecognitionOptions& GetOptions() { return *options_; }
  ObjectRecognitionRunner& GetRunner() { return *runner_; }

 private:
  std::shared_ptr<DictionaryObjectRecognitionOptions> options_;
  ObjectRecognitionRunner* runner_;
};

/////////////////////////////////////////////////////////////////////////////
class SetInstanceOptionsTask : public PromiseTask {
 public:
  SetInstanceOptionsTask();
  virtual ~SetInstanceOptionsTask();
  virtual void WorkerThreadExecute();
  // We don't need to override MainThreadDispatchResult(),
  // default behavior is good enough
  virtual InstanceOptionsTaskPayload* GetPayload();
  virtual v8_value_t GetRejected();
};

/////////////////////////////////////////////////////////////////////////////
class GetInstanceOptionsTask : public PromiseTask {
 public:
  GetInstanceOptionsTask();
  virtual ~GetInstanceOptionsTask();
  virtual void WorkerThreadExecute();
  virtual v8::Local<v8::Value> GetResolved();
  virtual InstanceOptionsTaskPayload* GetPayload();
};

/////////////////////////////////////////////////////////////////////////////
class CameraOptionsTaskPayload : public AsyncTaskPayload {
 public:
  CameraOptionsTaskPayload(
      std::shared_ptr<DictionaryCameraOptions> options,
      ObjectRecognitionRunner* runner) {
    options_ = options;
    runner_ = runner;
  }

  DictionaryCameraOptions& GetOptions() { return *options_; }
  ObjectRecognitionRunner& GetRunner() { return *runner_; }

  std::string GetMessage() const {
    return message_;
  }

  void SetMessage(const std::string& msg) {
    message_ = msg;
  }

 private:
  std::shared_ptr<DictionaryCameraOptions> options_;
  ObjectRecognitionRunner* runner_;
  std::string message_;
};

/////////////////////////////////////////////////////////////////////////////
class SetCameraOptionsTask : public PromiseTask {
 public:
  SetCameraOptionsTask();
  virtual ~SetCameraOptionsTask();
  virtual void WorkerThreadExecute();
  // We don't need to override MainThreadDispatchResult(),
  // default behavior is good enough
  virtual CameraOptionsTaskPayload* GetPayload();

  virtual v8_value_t GetRejected();
};

/////////////////////////////////////////////////////////////////////////////
class GetCameraOptionsTask : public PromiseTask {
 public:
  GetCameraOptionsTask();
  virtual ~GetCameraOptionsTask();
  virtual void WorkerThreadExecute();
  virtual v8::Local<v8::Value> GetResolved();
  virtual CameraOptionsTaskPayload* GetPayload();
};

/////////////////////////////////////////////////////////////////////////////
class FrameDataTaskPayload : public AsyncTaskPayload {
 public:
  explicit FrameDataTaskPayload(ObjectRecognitionRunner* runner) {
    runner_ = runner;
  }

  ObjectRecognitionRunner& GetRunner() { return *runner_; }

 private:
  ObjectRecognitionRunner* runner_;
};

/////////////////////////////////////////////////////////////////////////////
class GetFrameDataTask : public PromiseTask {
 public:
  GetFrameDataTask();
  virtual ~GetFrameDataTask();
  virtual void WorkerThreadExecute();
  // We don't need to override MainThreadDispatchResult(),
  // default behavior is good enough
  virtual v8::Local<v8::Value> GetResolved();
  virtual FrameDataTaskPayload* GetPayload();
};

/////////////////////////////////////////////////////////////////////////////
class RunObjectRecognitionTaskPayload : public AsyncTaskPayload {
  RunObjectRecognitionTaskPayload(const RunObjectRecognitionTaskPayload&)
      = delete;
  RunObjectRecognitionTaskPayload& operator =
      (const RunObjectRecognitionTaskPayload&) = delete;

 public:
  explicit RunObjectRecognitionTaskPayload(ObjectRecognitionRunner* runner) {
    runner_ = runner;
  }
  virtual ~RunObjectRecognitionTaskPayload() {}

  ObjectRecognitionRunner& GetRunner() { return *runner_; }

 private:
  ObjectRecognitionRunner* runner_;
};

/////////////////////////////////////////////////////////////////////////////
class RunObjectRecognitionTask : public EventEmitterTask {
 public:
  RunObjectRecognitionTask();
  virtual ~RunObjectRecognitionTask();

  virtual void ReloadIfNeeded();
  virtual void WorkerThreadExecute();
  // TODO(Kenny): deal with the const mod in EventEmitterTask
  virtual size_t GetEventCount();
  virtual std::string GetEventName(size_t event_index) const;
  virtual bool ShouldPopEvent(size_t event_index);
  virtual v8_value_t PopEventData(size_t event_index);
  virtual void RequestCancel();
  // virtual void RequestRestart ();
  virtual void RequestPause();
  virtual void RequestResume();

  virtual RunObjectRecognitionTaskPayload* GetPayload();

 protected:
  virtual void OnPayloadSet(AsyncTaskPayload* payload);

 private:
  class RunObjectRecognitionTaskD* d_;
};

/////////////////////////////////////////////////////////////////////////////
class ObjectNamesTaskPayload : public AsyncTaskPayload {
 public:
  explicit ObjectNamesTaskPayload(ObjectRecognitionRunner* runner) {
    runner_ = runner;
  }

  std::vector<std::string>& GetNamesList() { return names_; }

  ObjectRecognitionRunner* GetRunner() { return runner_; }

 private:
  ObjectRecognitionRunner* runner_;
  std::vector<std::string> names_;
};

/////////////////////////////////////////////////////////////////////////////
class GetRecognizableObjectNamesTask : public PromiseTask {
 public:
  GetRecognizableObjectNamesTask();
  virtual ~GetRecognizableObjectNamesTask();
  virtual void WorkerThreadExecute();
  virtual ObjectNamesTaskPayload* GetPayload();
  virtual v8_value_t GetResolved();
  virtual v8_value_t GetRejected();
};

#endif  // _OBJECT_RECOGNITION_TASKS_H_
