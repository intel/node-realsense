// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/person_tracker_creator.h"

#include "gen/nan__person_tracker.h"

Nan::Persistent<v8::Object>* PersonTrackerCreator::tracker_object_ = nullptr;

static bool IsPlaceholder(v8::Local<v8::Value> value) {
  return value->IsUndefined() || value->IsNull();
}

NAN_METHOD(CreatePersonTracker) {
  DictionaryPersonTrackerOptions dic_tracker_options;
  DictionaryCameraOptions dic_camera_options;

  bool tracker_options_present = false;
  bool camera_options_present = false;
  bool wrong_arguments = false;

  // Quite lame way to process arguments, but it works
  int32_t len = info.Length();
  for (int32_t i=0; i < len; i++) {
    if (!info[i]->IsObject() && !IsPlaceholder(info[i])) {
      wrong_arguments = true;
      break;
    }
  }

  if (wrong_arguments) {
    info.GetReturnValue().Set(PersonTrackerAdapter::CreateRejectedPromise(
        "Invalid CreatePersonTracker parameters"));
    return;
  }

  if (len >= 1 && info[0]->IsObject())
    tracker_options_present = true;

  if (len == 2 && info[1]->IsObject())
    camera_options_present = true;

  // Check the option value type
  std::string error_string;
  if (tracker_options_present) {
    PersonTrackerOptions tracker_options(info[0]->ToObject());
    dic_tracker_options.ImportFrom(tracker_options);

    if (!tracker_options.CheckType(&error_string)) {
      info.GetReturnValue().Set(
          PersonTrackerAdapter::CreateRejectedPromise(error_string));
      return;
    }
  }

  if (camera_options_present) {
    CameraOptions camera_options(info[0]->ToObject());
    dic_camera_options.ImportFrom(camera_options);

    if (!camera_options.CheckType(&error_string)) {
      info.GetReturnValue().Set(
          PersonTrackerAdapter::CreateRejectedPromise(error_string));
      return;
    }
  }

  PersonTrackerCreator* creator = new PersonTrackerCreator();
  if (tracker_options_present)
    creator->SetTrackerOptions(dic_tracker_options);
  if (camera_options_present)
    creator->SetCameraOptions(dic_camera_options);
  creator->CreatePromiseAndSetFunctionReturnValue(info);
}

PersonTrackerCreator::PersonTrackerCreator()
    : have_tracker_options_(false),
      have_camera_options_(false) {
}

void PersonTrackerCreator::SetTrackerOptions(
      const DictionaryPersonTrackerOptions& tracker_options) {
  tracker_options_ = tracker_options;
  have_tracker_options_ = true;
}

void PersonTrackerCreator::SetCameraOptions(
    const DictionaryCameraOptions& camera_options) {
  camera_options_ = camera_options;
  have_camera_options_ = true;
}

bool PersonTrackerCreator::DoWork() {
  bool success = true;
  if (have_tracker_options_)
    success = PersonTrackerRunnerProxy::GetInstance()->SetPersonTrackerOptionsDirectly(  // NOLINT
        tracker_options_, &fail_reason_);
  if (success && have_camera_options_)
    success = PersonTrackerRunnerProxy::GetInstance()->SetCameraOptionsDirectly(  // NOLINT
        camera_options_, &fail_reason_);
  return success;
}

void PersonTrackerCreator::OnWorkDone() {
  PromiseWorkQueueHelper::OnWorkDone();
  v8::Isolate::GetCurrent()->RunMicrotasks();
}

v8::Local<v8::Value> PersonTrackerCreator::GetResolved() {
  if (!tracker_object_) {
    tracker_object_ = new Nan::Persistent<v8::Object>();
    tracker_object_->Reset(NanPersonTracker::NewInstance());
  }
  return Nan::New(*tracker_object_);
}

v8::Local<v8::Value> PersonTrackerCreator::GetRejected() {
  return Nan::New(fail_reason_).ToLocalChecked();
}

void PersonTrackerCreator::DestroyInstance() {
  if (tracker_object_) {
    tracker_object_->Reset();
  }
  delete tracker_object_;
  tracker_object_ = nullptr;
}
