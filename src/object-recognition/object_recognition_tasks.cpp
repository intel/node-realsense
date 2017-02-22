// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "object_recognition_tasks.h"

#include "common/camera-options/camera_options_host_instance.h"

/////////////////////////////////////////////////////////////////////////////
SetInstanceOptionsTask::SetInstanceOptionsTask() {
  task_tag = "setInstanceOptions() task";
}

SetInstanceOptionsTask::~SetInstanceOptionsTask() {
}

void SetInstanceOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  auto options = payload->GetOptions();

  if (payload->GetRunner().ImportOptions(options)) {
    // Everything went smoothly
    task_state = Successful;
  } else {
    // TODO(Kenny): assemble a better error message
    task_state = Failed;
  }
}

InstanceOptionsTaskPayload* SetInstanceOptionsTask::GetPayload() {
  return reinterpret_cast<InstanceOptionsTaskPayload*>(AsyncTask::GetPayload());
}

v8_value_t SetInstanceOptionsTask::GetRejected() {
  return Nan::New("Invalid Options").ToLocalChecked();
}

/////////////////////////////////////////////////////////////////////////////
GetInstanceOptionsTask::GetInstanceOptionsTask() {
  task_tag = "getInstanceOptions() task";
}

GetInstanceOptionsTask::~GetInstanceOptionsTask() {
}

void GetInstanceOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  payload->GetRunner().ExportOptions(&payload->GetOptions());
  task_state = Successful;
}

v8::Local<v8::Value> GetInstanceOptionsTask::GetResolved() {
  ObjectRecognitionOptions dictionary;
  auto& options = GetPayload()->GetOptions();
  options.ExportTo(dictionary);

  return static_cast<v8::Local<v8::Object>>(dictionary);
}

InstanceOptionsTaskPayload* GetInstanceOptionsTask::GetPayload() {
  return reinterpret_cast<InstanceOptionsTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
SetCameraOptionsTask::SetCameraOptionsTask() {
  task_tag = "setCameraOptions() task";
}

SetCameraOptionsTask::~SetCameraOptionsTask() {
}

void SetCameraOptionsTask::WorkerThreadExecute() {
  auto payload = GetPayload();
  auto options = payload->GetOptions();

  // Apply options
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  CameraOptionsType previous = host->GetCameraOptions();
  host->PartiallyOverwriteCameraOptions(options);

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
    payload->SetMessage(message);
    host->SetCameraOptions(previous);
  } catch (...) {
    task_state = Failed;
    host->SetCameraOptions(previous);
  }
}

CameraOptionsTaskPayload* SetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}

v8_value_t SetCameraOptionsTask::GetRejected() {
  auto payload = GetPayload();
  return Nan::New(payload->GetMessage()).ToLocalChecked();
}

/////////////////////////////////////////////////////////////////////////////
GetCameraOptionsTask::GetCameraOptionsTask() {
  task_tag = "getCameraOptions() task";
}

GetCameraOptionsTask::~GetCameraOptionsTask() {
}

void GetCameraOptionsTask::WorkerThreadExecute() {
  auto& options = GetPayload()->GetOptions();

  // Retrieve options
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  options = host->GetCameraOptions();

  task_state = Successful;
}

v8::Local<v8::Value> GetCameraOptionsTask::GetResolved() {
  CameraOptions dictionary;
  auto& options = GetPayload()->GetOptions();
  options.ExportTo(dictionary);

  return static_cast<v8::Local<v8::Object>>(dictionary);
}

CameraOptionsTaskPayload* GetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
GetFrameDataTask::GetFrameDataTask() {
}

GetFrameDataTask::~GetFrameDataTask() {
}

void GetFrameDataTask::WorkerThreadExecute() {
  task_state = AsyncTask::Successful;
  return;
}

v8::Local<v8::Value> GetFrameDataTask::GetResolved() {
  return GetPayload()->GetRunner().GetFrameData();
}

FrameDataTaskPayload* GetFrameDataTask::GetPayload() {
  return reinterpret_cast<FrameDataTaskPayload*>(AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
class RunObjectRecognitionTaskD {
  RunObjectRecognitionTaskD() {
    continue_running_or = true;
    paused_running_or = false;
    initialized = false;
    stop_request_processed = false;
  }

  bool continue_running_or;
  bool paused_running_or;
  bool initialized;
  bool stop_request_processed;

  friend class RunObjectRecognitionTask;
};

/////////////////////////////////////////////////////////////////////////////
enum RunObjectRecognitionEventIndex {
  FrameCaptured,
  FrameProcessed,
  ObjectRecognitionResult,
  RunObjectRecognitionEventCount
};

/////////////////////////////////////////////////////////////////////////////
RunObjectRecognitionTask::RunObjectRecognitionTask() {
  task_tag = "RunObjectRecognitionTask task";
  d_ = new RunObjectRecognitionTaskD();
}

RunObjectRecognitionTask::~RunObjectRecognitionTask() {
  delete d_;
}

void RunObjectRecognitionTask::ReloadIfNeeded() {
  if (task_state == AsyncTask::Failed) {
    // Never reload a failed task, it's dead
    return;
  }

  if (d_->continue_running_or) {
    if (d_->paused_running_or) {
      GetPayload()->GetRunner().SetInstanceState(OR_STATE_PAUSED);
      task_state = AsyncTask::Pending;
    } else {
      task_state = AsyncTask::Ready;
    }
  } else if (!d_->stop_request_processed) {
    // Reload this task to make underlying software stop
    task_state = AsyncTask::Ready;
  }
}

void RunObjectRecognitionTask::WorkerThreadExecute() {
  if (!d_->continue_running_or) {
    GetPayload()->GetRunner().Stop();
    GetPayload()->GetRunner().SetInstanceState(OR_STATE_READY);
    task_state = AsyncTask::Successful;
    d_->stop_request_processed = true;
    return;
  }

  auto& runner = GetPayload()->GetRunner();

  std::string error_msg;
  try {
    if (!d_->initialized) {
      CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
      host->Validate();
      host->WriteToCamera();
      if (!runner.Init()) {
        throw std::string("Failed to initialize camera");
      }
      d_->initialized = true;
    }

    if (!d_->paused_running_or) {
      runner.SetInstanceState(OR_STATE_RUNNING);
      runner.Run();
    } else {
      runner.SetInstanceState(OR_STATE_PAUSED);
    }
  } catch (const std::string& error) {
    error_msg = error;
  } catch (...) {
    error_msg = "Unknown exception caught";
  }

  if (error_msg.length()) {
    task_state = AsyncTask::Failed;
    runner.SetInstanceState(OR_STATE_ERRORED);
  } else {
    task_state = AsyncTask::Successful;
  }
}

size_t RunObjectRecognitionTask::GetEventCount() {
  if (task_state == AsyncTask::Failed) {
    return 0;
  }
  return static_cast<size_t>(RunObjectRecognitionEventCount);
}

std::string RunObjectRecognitionTask::GetEventName(size_t event_index) const {
  switch (event_index) {
  case FrameCaptured:
    return "framecaptured";
  case FrameProcessed:
    return "frameprocessed";
  case ObjectRecognitionResult:
    // TODO(Kenny): deal this const issue in the framework level
    return (const_cast<RunObjectRecognitionTask*>(this))
        ->GetPayload()->GetRunner().GetEventName();
  default:
    return "<no-such-event>";  // Won't be invoked
  }
}

bool RunObjectRecognitionTask::ShouldPopEvent(size_t event_index) {
  switch (event_index) {
  case FrameCaptured:
    return true;
  case FrameProcessed:
    return GetPayload()->GetRunner().IsLastFrameProcessed();
  case ObjectRecognitionResult:
    return GetPayload()->GetRunner().IsResultReady() && d_->continue_running_or;
  default:
    return false;
  }
}

v8_value_t RunObjectRecognitionTask::PopEventData(size_t event_index) {
  switch (event_index) {
  case FrameCaptured:
  case FrameProcessed:
    return Nan::Undefined();
  case ObjectRecognitionResult:
    return GetPayload()->GetRunner().GetResult();
  default:
    return Nan::Undefined();
  }
}

void RunObjectRecognitionTask::RequestPause() {
  d_->paused_running_or = true;
}

void RunObjectRecognitionTask::RequestResume() {
  d_->paused_running_or = false;
  if (task_state == AsyncTask::Pending) {
    task_state = AsyncTask::Ready;
  }
}

void RunObjectRecognitionTask::OnPayloadSet(AsyncTaskPayload* payload) {
}

void RunObjectRecognitionTask::RequestCancel() {
  d_->continue_running_or = false;
}

RunObjectRecognitionTaskPayload* RunObjectRecognitionTask::GetPayload() {
  return reinterpret_cast<RunObjectRecognitionTaskPayload*>
      (AsyncTask::GetPayload());
}

/////////////////////////////////////////////////////////////////////////////
GetRecognizableObjectNamesTask::GetRecognizableObjectNamesTask() {
}

GetRecognizableObjectNamesTask::~GetRecognizableObjectNamesTask() {
}

void GetRecognizableObjectNamesTask::WorkerThreadExecute() {
  if (auto payload = GetPayload()) {
    auto runner = payload->GetRunner();
    if (runner->DumpRecognizableObjectNames(&payload->GetNamesList())) {
      task_state = Successful;
      return;
    }
  }
  task_state = Failed;
}

ObjectNamesTaskPayload* GetRecognizableObjectNamesTask:: GetPayload() {
  return reinterpret_cast<ObjectNamesTaskPayload*>
    (AsyncTask::GetPayload());
}

v8_value_t GetRecognizableObjectNamesTask::GetResolved() {
  if (auto payload = GetPayload()) {
    const auto& names = payload->GetNamesList();;
    ArrayHelper array;
    for (size_t i = 0 ; i < names.size() ; ++i) {
      array.Set(i, names[i]);
    }
    return static_cast<v8::Local<v8::Array>>(array);
  }
  return Nan::Undefined();
}

v8_value_t GetRecognizableObjectNamesTask::GetRejected() {
  return Nan::New("Failed to obtain list of recognizable object").
    ToLocalChecked();
}
