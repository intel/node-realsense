// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <string>

#include "instance_creator.h"

#include "gen/nan__object_recognizer.h"

#include "object_recognition_runner.h"
#include "common/camera-options/camera_options_host_instance.h"

class InstanceCreatorD {
  InstanceCreatorD() {}
  ~InstanceCreatorD() {}

  friend class InstanceCreator;
  DictionaryObjectRecognitionOptions  instance_options_;
  DictionaryCameraOptions             camera_options_;
  std::string                         failed_msg_;
};

static Nan::Persistent<v8::Object>* instance_ = nullptr;

static bool IsPlaceholder(v8::Local<v8::Value> value) {
  return value->IsUndefined() || value->IsNull();
}

NAN_METHOD(CreateObjectRecognizerInstance) {
  DictionaryObjectRecognitionOptions instance_options;
  DictionaryCameraOptions camera_options;

  bool instance_options_present = false;
  bool camera_options_present = false;
  bool wrong_arguments = true;

  // Quite lame way to process arguments, but it works
  if (info.Length() == 1 && info[0]->IsObject()) {
    instance_options_present = true;
    wrong_arguments = false;
  } else if (info.Length() == 2) {
    if (info[0]->IsObject()) {
      instance_options_present = true;

      if (info[1]->IsObject() || IsPlaceholder(info[1])) {
        wrong_arguments = false;
      }
    }

    if (info[1]->IsObject()) {
      camera_options_present = true;
      if (info[0]->IsObject() || IsPlaceholder(info[0])) {
        wrong_arguments = false;
      }
    }

    if (IsPlaceholder(info[0]) && IsPlaceholder(info[1])) {
      wrong_arguments = false;
    }
  } else if (info.Length() == 0) {
    wrong_arguments = false;
  }

  if (wrong_arguments) {
    // TODO(Kenny): refine error message
    std::string message = "function takes 2 optional arguments (optional ObjectRecognitionOptions, optional CameraOptions)"; // NOLINT
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise(message);
    info.GetReturnValue().Set(promise);
    return;
  }

  if (instance_options_present) {
    instance_options.ImportFrom(v8::Local<v8::Object>::Cast(info[0]));
  }

  if (camera_options_present) {
    camera_options.ImportFrom(v8::Local<v8::Object>::Cast(info[1]));
  }

  InstanceCreator* creator = new InstanceCreator(
      instance_options,
      camera_options);
  creator->CreatePromiseAndSetFunctionReturnValue(info);
}

InstanceCreator::InstanceCreator(
    const DictionaryObjectRecognitionOptions& instance_options,
    const DictionaryCameraOptions& camera_options) {
  d_ = new InstanceCreatorD();
  d_->instance_options_ = instance_options;
  d_->camera_options_ = camera_options;
}

InstanceCreator::~InstanceCreator() {
  delete d_;
}

bool InstanceCreator::DoWork() {
  bool success = true;

  auto runner = ObjectRecognitionRunner::GetRunnerInstance();
  runner->Reset();

  // 1. InstanceOptions
  runner->ImportOptions(d_->instance_options_);

  // 2. CameraOptions
  CameraOptionsHost* host = CameraOptionsHostInstance::GetInstance();
  CameraOptionsType previous = host->GetCameraOptions();
  host->PartiallyOverwriteCameraOptions(d_->camera_options_);
  try {
    if (host->Validate()) {
      host->WriteToCamera();
      success = true;
    } else {
      auto message = host->GetValidationFailureMessage();
      throw message;
    }
  } catch (const std::string& message) {
    success = false;
    d_->failed_msg_ = message;
    host->SetCameraOptions(previous);
  } catch (...) {
    success = false;
    d_->failed_msg_ = "Wrong arguments provided";
    host->SetCameraOptions(previous);
  }

  return success;
}

void InstanceCreator::OnWorkDone() {
  PromiseWorkQueueHelper::OnWorkDone();
  v8::Isolate::GetCurrent()->RunMicrotasks();
}

v8::Local<v8::Value> InstanceCreator::GetResolved() {
  if (!instance_) {
    instance_ = new Nan::Persistent<v8::Object>();
    instance_->Reset(NanObjectRecognizer::NewInstance());
  }
  return Nan::New(*instance_);
}

v8::Local<v8::Value> InstanceCreator::GetRejected() {
  return Nan::New(d_->failed_msg_.c_str()).ToLocalChecked();
}

void InstanceCreator::DestroyInstance() {
  if (instance_) {
    instance_->Reset();
  }
  delete instance_;
  instance_ = nullptr;
}
