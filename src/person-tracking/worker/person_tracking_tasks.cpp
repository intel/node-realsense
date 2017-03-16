// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/person_tracking_tasks.h"

#include "common/camera-options/camera_options_host_instance.h"
#include "gen/array_helper.h"
#include "gen/nan__person_info.h"
#include "gen/nan__person_recognizer_data.h"
#include "gen/nan__person_recognizer_data_with_status.h"
#include "gen/nan__person_registration_data.h"

using RecognitionType = PT::PersonTrackingData::PersonRecognition;

PTAsyncTaskPayload* PTPromiseTask::GetPayload() {
  return reinterpret_cast<PTAsyncTaskPayload*>(AsyncTask::GetPayload());
}

PersonTrackerAdapter* PTPromiseTask::GetAdapter() {
  PTAsyncTaskPayload* payload = GetPayload();
  return payload->GetAdapter();
}

v8_value_t PTPromiseTask::GetRejected() {
  return Nan::New(reject_reason_).ToLocalChecked();
}

PTAsyncTaskPayload* PTEventEmitterTask::GetPayload() {
  return reinterpret_cast<PTAsyncTaskPayload*>(AsyncTask::GetPayload());
}

PersonTrackerAdapter* PTEventEmitterTask::GetAdapter() {
  PTAsyncTaskPayload*  payload = GetPayload();
  return payload->GetAdapter();
}

void SetOptionsTask::WorkerThreadExecute() {
  SetOptionsTaskPayload* payload = GetPayload();
  PersonTrackerAdapter* adapter = GetAdapter();
  if (adapter->SetConfig(payload->GetOptions(), &reject_reason_))
    task_state = AsyncTask::Successful;
  else
    task_state = AsyncTask::Failed;
}

SetOptionsTaskPayload* SetOptionsTask::GetPayload() {
  return reinterpret_cast<SetOptionsTaskPayload*>(AsyncTask::GetPayload());
}

void GetOptionsTask::WorkerThreadExecute() {
  auto adapter = GetAdapter();
  if (adapter->HaveConfig()) {
    task_state = AsyncTask::Successful;
  } else {
    task_state = AsyncTask::Failed;
    reject_reason_ = "Have not been configured";
  }
}

v8_value_t GetOptionsTask::GetResolved() {
  auto adapter = GetAdapter();
  PersonTrackerOptions options;
  if (adapter->GetConfig(&options))
    return static_cast<v8::Local<v8::Object>>(options);
  else
    return Nan::Undefined();
}

void SetCameraOptionsTask::WorkerThreadExecute() {
  CameraOptionsTaskPayload* payload = GetPayload();
  auto options = payload->GetOptions();

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
    reject_reason_ = message;
    host->SetCameraOptions(previous);
  } catch (...) {
    task_state = Failed;
    reject_reason_ = "Failed to set camera options";
    host->SetCameraOptions(previous);
  }
}

CameraOptionsTaskPayload* SetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}

void GetCameraOptionsTask::WorkerThreadExecute() {
  CameraOptionsTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  payload->options_ = std::make_shared<DictionaryCameraOptions>(
      *(adapter->GetCameraOptions()));
  task_state = Successful;
}

v8_value_t GetCameraOptionsTask::GetResolved() {
  CameraOptions dictionary;
  CameraOptionsTaskPayload* payload = GetPayload();
  auto options = payload->GetOptions();
  if (options) {
    options->ExportTo(dictionary);
    return static_cast<v8::Local<v8::Object>>(dictionary);
  }
  return Nan::Undefined();
}

CameraOptionsTaskPayload* GetCameraOptionsTask::GetPayload() {
  return reinterpret_cast<CameraOptionsTaskPayload*>(AsyncTask::GetPayload());
}

void StartPersonTrackingTask::WorkerThreadExecute() {
  auto adapter = GetAdapter();
  if (adapter->Start())
    task_state = AsyncTask::Successful;
  else
    task_state = AsyncTask::Failed;
}

void RunPersonTrackingTask::WorkerThreadExecute() {
  auto adapter = GetAdapter();

  // check user's cancel request
  if (request_cancel_) {
    adapter->Stop();
    task_state = AsyncTask::Canceled;
    return;
  }

  if (request_pause_) {
    adapter->Pause();
    task_state = AsyncTask::Pending;
    return;
  }

  if (request_resume_ &&
      adapter->GetState() == PersonTrackerAdapter::kStatePaused) {
    adapter->Resume();
    request_resume_ = false;
  }

  // The Start is called each time to check whether the camera and the
  // module have been configured, and will configure them and start the camera
  // if not.
  if (adapter->Start() && adapter->IterationWork()) {
    error_happen_ = false;
    task_state = AsyncTask::Successful;
  }  else {
    error_happen_ = true;
    task_state = AsyncTask::Failed;
  }
}

void RunPersonTrackingTask::MainThreadDispatchResult() {
  // If the task has been canceled, no longer send event.
  if (task_state == AsyncTask::Canceled)
    return;
  auto adapter = GetAdapter();
  PersonTrackerAdapter::SafeGuard guard;
  // If there is no error, then is_running is true, we should prepare the result
  // for sending the events.
  adapter->BeforeEmitResult();

  // call father method to emit events.
  EventEmitterTask::MainThreadDispatchResult();

  adapter->AfterEmitResult();
}

void RunPersonTrackingTask::ReloadIfNeeded() {
  // If user has called stop, not reload it again.
  if (task_state == AsyncTask::Canceled)
    return;

  // Make it ready, it will be reloaded. If error happened, its state has
  // been set in WorkerThreadExecute
  if (!error_happen_)
    task_state = AsyncTask::Ready;
}

bool RunPersonTrackingTask::ShouldPopEvent(size_t event_index) {
  auto adapter = GetAdapter();
  return adapter->ShouldPopEvent(event_index);
}

v8_value_t RunPersonTrackingTask::PopEventData(size_t event_index) {
  auto adapter = GetAdapter();
  return adapter->PopEventData(event_index);
}

std::string RunPersonTrackingTask::GetEventName(size_t event_index) const {
  auto adapter = (const_cast<RunPersonTrackingTask*>(this))->GetAdapter();
  return adapter->GetEventName(event_index);
}

size_t RunPersonTrackingTask::GetEventCount() {
  auto adapter = GetAdapter();
  return adapter->GetEventCount();
}

// This method is called in main thread when user calls stop.
void RunPersonTrackingTask::RequestCancel() {
  request_cancel_ = true;
}

void RunPersonTrackingTask::RequestPause() {
  request_pause_ = true;
}

void RunPersonTrackingTask::RequestResume() {
  if (task_state == AsyncTask::Pending) {
    request_pause_ = false;
    request_resume_ = true;
    task_state = AsyncTask::Ready;
  }
}

void ResetTask::WorkerThreadExecute() {
  auto adapter = GetAdapter();
  adapter->Reset();
  task_state = AsyncTask::Successful;
}

void StartOrStopTrackingOnePersonTask::WorkerThreadExecute() {
  auto adapter = GetAdapter();
  StartOrStopTrackingOnePersonTaskPayload* payload = GetPayload();
  if (payload->is_start_) {
    if (adapter->StartTracking(payload->id_))
      task_state = AsyncTask::Successful;
    else
      task_state = AsyncTask::Failed;
  } else {
    if (adapter->StopTracking(payload->id_))
      task_state = AsyncTask::Successful;
    else
      task_state = AsyncTask::Failed;
  }
}

StartOrStopTrackingOnePersonTaskPayload* StartOrStopTrackingOnePersonTask
    ::GetPayload() {
  return reinterpret_cast<StartOrStopTrackingOnePersonTaskPayload*>(
      AsyncTask::GetPayload());
}

void GetPersonInfoTask::WorkerThreadExecute() {
  GetPersonInfoTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  std::shared_ptr<PersonDataInternal> data =
      adapter->GetPersonData(payload->id_);
  if (data) {
    payload->person_data_ = data;
    task_state = Successful;
  } else {
    task_state = Failed;
    reject_reason_ = "No person data";
  }
}

v8_value_t GetPersonInfoTask::GetResolved() {
  GetPersonInfoTaskPayload* payload = GetPayload();
  if (payload->person_data_) {
    PersonInfo* info = payload->person_data_->ToPersonInfo();
    payload->person_data_.reset();
    auto wrapper = NanPersonInfo::NewInstance(info);
    return wrapper;
  }
  return Nan::Undefined();
}

GetPersonInfoTaskPayload* GetPersonInfoTask::GetPayload() {
  return reinterpret_cast<GetPersonInfoTaskPayload*>(AsyncTask::GetPayload());
}

void RegisterPersonTask::WorkerThreadExecute() {
  RegisterPersonTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->RegisterPerson(
      payload->id_, &(payload->result_), &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

RegisterPersonTaskPayload* RegisterPersonTask::GetPayload() {
  return reinterpret_cast<RegisterPersonTaskPayload*>(AsyncTask::GetPayload());
}

v8_value_t RegisterPersonTask::GetResolved() {
  RegisterPersonTaskPayload* payload = GetPayload();
  return NanPersonRegistrationData::NewInstance(
      new PersonRegistrationData(payload->result_));
}

void RecognizePersonTask::WorkerThreadExecute() {
  RecognizePersonTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->RecognizePerson(
      payload->id_, &(payload->result_), &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

RecognizePersonTaskPayload* RecognizePersonTask::GetPayload() {
  return reinterpret_cast<RecognizePersonTaskPayload*>(
      AsyncTask::GetPayload());
}

v8_value_t RecognizePersonTask::GetResolved() {
  RecognizePersonTaskPayload* payload = GetPayload();
  auto wrapper = NanPersonRecognizerData::NewInstance(
      new PersonRecognizerData(payload->result_));
  return wrapper;
}

void RecognizeAllPersonTask::WorkerThreadExecute() {
  RecognizeAllPersonTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->RecognizeAllPersons(&(payload->result_), &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

RecognizeAllPersonTaskPayload* RecognizeAllPersonTask::GetPayload() {
  return reinterpret_cast<RecognizeAllPersonTaskPayload*>(
      AsyncTask::GetPayload());
}

v8_value_t RecognizeAllPersonTask::GetResolved() {
  RecognizeAllPersonTaskPayload* payload = GetPayload();
  ArrayHelper results;
  results.FromArrayOfImplT<NanPersonRecognizerDataWithStatus>(
      payload->result_.begin(), payload->result_.end());
  return (v8::Local<v8::Array>)results;
}

void UnRegisterPersonTask::WorkerThreadExecute() {
  UnRegisterPersonTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->UnRegisterPerson(payload->id_, &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

UnRegisterPersonTaskPayload* UnRegisterPersonTask::GetPayload() {
  return reinterpret_cast<UnRegisterPersonTaskPayload*>(
      AsyncTask::GetPayload());
}

void GetAllRecognitionIDsTask::WorkerThreadExecute() {
  GetAllRecognitionIDsTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->GetRecognitionIDs(&payload->result_, &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

v8_value_t GetAllRecognitionIDsTask::GetResolved() {
  GetAllRecognitionIDsTaskPayload* payload = GetPayload();
  ArrayHelper results;
  results.FromArrayT(payload->result_.begin(), payload->result_.end());
  return (v8::Local<v8::Array>)results;
}

GetAllRecognitionIDsTaskPayload* GetAllRecognitionIDsTask::GetPayload() {
  return reinterpret_cast<GetAllRecognitionIDsTaskPayload*>(
      AsyncTask::GetPayload());
}

void RecognitionIDExistTask::WorkerThreadExecute() {
  RecognitionIDExistTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->RecognitionIDExist(
      payload->id_, &payload->exist_, &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

RecognitionIDExistTaskPayload* RecognitionIDExistTask::GetPayload() {
  return reinterpret_cast<RecognitionIDExistTaskPayload*>(
      AsyncTask::GetPayload());
}

v8_value_t RecognitionIDExistTask::GetResolved() {
  RecognitionIDExistTaskPayload* payload = GetPayload();
  return Nan::New(payload->exist_);
}

void GetPersonDescriptorIDsTask::WorkerThreadExecute() {
  GetPersonDescriptorIDsTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->GetRegistrationDescriptorIDs(
      payload->id_, &payload->result_, &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

v8_value_t GetPersonDescriptorIDsTask::GetResolved() {
  GetPersonDescriptorIDsTaskPayload* payload = GetPayload();
  ArrayHelper results;
  results.FromArrayT(payload->result_.begin(), payload->result_.end());
  return (v8::Local<v8::Array>)results;
}

GetPersonDescriptorIDsTaskPayload* GetPersonDescriptorIDsTask::GetPayload() {
  return reinterpret_cast<GetPersonDescriptorIDsTaskPayload*>(
      AsyncTask::GetPayload());
}

void RemovePersonDescriptorTask::WorkerThreadExecute() {
  RemovePersonDescriptorTaskPayload* payload = GetPayload();
  auto adapter = GetAdapter();
  if (adapter->RemoveRegistrationDescriptor(
      payload->id_, payload->descriptor_id_, &reject_reason_)) {
    task_state = Successful;
  } else {
    task_state = Failed;
  }
}

RemovePersonDescriptorTaskPayload* RemovePersonDescriptorTask::GetPayload() {
  return reinterpret_cast<RemovePersonDescriptorTaskPayload*>(
      AsyncTask::GetPayload());
}
