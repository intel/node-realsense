// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "object_recognizer.h"

#include <memory>

#include "common/task/async_task_runner_instance.h"
#include "gen/promise-helper.h"
#include "object_recognition_runner.h"
#include "object_recognition_tasks.h"

class ObjectRecognizerD {
  ObjectRecognizerD() {
    runner_ = ObjectRecognitionRunner::GetRunnerInstance();
    runner_->SetInstanceState(OR_STATE_READY);
  }

  ~ObjectRecognizerD() {
    delete runner_;
  }

  std::string or_task;
  ObjectRecognitionRunner * runner_;
  Nan::Persistent<v8::Object> js_this_;

  friend class ObjectRecognizer;
};

ObjectRecognizer::ObjectRecognizer() {
  d_ = new ObjectRecognizerD();
}

ObjectRecognizer::~ObjectRecognizer() {
  delete d_;
}

ObjectRecognizer& ObjectRecognizer::operator = (const ObjectRecognizer& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

void ObjectRecognizer::SetJavaScriptThis(v8::Local<v8::Object> obj) {
  d_->js_this_.Reset(obj);
}

std::string ObjectRecognizer::get_state() const {
  return d_->runner_->GetInstanceState();
}

v8::Local<v8::Promise> ObjectRecognizer::getCameraOptions() {
  auto payload = new CameraOptionsTaskPayload(
      std::make_shared<DictionaryCameraOptions>(), d_->runner_);

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetCameraOptionsTask(),
      payload,
      "{{GET_CAMERA_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::setCameraOptions(
    const CameraOptions& options) {
  auto payload = new CameraOptionsTaskPayload(
      std::make_shared<DictionaryCameraOptions>(), d_->runner_);
  payload->GetOptions().ImportFrom(options);
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SetCameraOptionsTask(),
      payload,
      "{{SET_CAMERA_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::getObjectRecognitionOptions() {
  auto payload = new InstanceOptionsTaskPayload(
      std::make_shared<DictionaryObjectRecognitionOptions>(), d_->runner_);

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetInstanceOptionsTask(),
      payload,
      "{{GET_INSTANCE_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::setObjectRecognitionOptions(
    const ObjectRecognitionOptions& options) {
  auto payload = new InstanceOptionsTaskPayload(
      std::make_shared<DictionaryObjectRecognitionOptions>(), d_->runner_);
  payload->GetOptions().ImportFrom(options);

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SetInstanceOptionsTask(),
      payload,
      "{{SET_INSTANCE_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::start() {
  if (get_state() != OR_STATE_READY) {
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise();
    return promise;
  }

  auto task = new RunObjectRecognitionTask();
  d_->or_task = task->unique_id;
  auto payload = new RunObjectRecognitionTaskPayload(d_->runner_);
  return AsyncTaskRunnerInstance::GetInstance()->PostEventEmitterTask(task,
      payload,
      Nan::New(d_->js_this_),
      "{{RUN_OR_TASK MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::pause() {
  if (get_state() != OR_STATE_RUNNING) {
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise();
    return promise;
  }

  return AsyncTaskRunnerInstance::GetInstance()->RequestPauseTask(d_->or_task);
}

v8::Local<v8::Promise> ObjectRecognizer::resume() {
  if (get_state() != OR_STATE_PAUSED) {
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise();
    return promise;
  }

  return AsyncTaskRunnerInstance::GetInstance()
    ->RequestResumeTask(d_->or_task);
}

v8::Local<v8::Promise> ObjectRecognizer::stop() {
  if (get_state() == OR_STATE_READY) {
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise();
    return promise;
  }

  return AsyncTaskRunnerInstance::GetInstance()
    ->RequestCancelTask(d_->or_task);
}

v8::Local<v8::Promise> ObjectRecognizer::reset() {
  PromiseHelper helper;
  auto promise = helper.CreatePromise();
  helper.RejectPromise(Nan::New("Not supported yet").ToLocalChecked());
  return promise;
}

v8::Local<v8::Promise> ObjectRecognizer::restart() {
  PromiseHelper helper;
  auto promise = helper.CreatePromise();
  helper.RejectPromise(Nan::New("Not supported yet").ToLocalChecked());
  return promise;
}

v8::Local<v8::Promise> ObjectRecognizer::getFrameData() {
  if (get_state() != OR_STATE_RUNNING) {
    PromiseHelper resolver;
    auto promise = resolver.CreatePromise();
    resolver.RejectPromise(std::string("<object recognition not running>"));
    return promise;
  }

  auto payload = new FrameDataTaskPayload(d_->runner_);
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetFrameDataTask(),
      payload,
      "{{GET_FRAME_DATA MESSAGE}}");
}

v8::Local<v8::Promise> ObjectRecognizer::getRecognizableObjectNames() {
  auto task = new GetRecognizableObjectNamesTask();
  auto payload = new ObjectNamesTaskPayload(d_->runner_);
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(task,
      payload,
      "{{OBJECT_NAMES_TASK}}");
}
