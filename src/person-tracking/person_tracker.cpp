// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_tracker.h"
#include "gen/nan__frame_data.h"

PersonTracker::PersonTracker() {
  PersonTrackerRunnerProxy::AddReference();
  PersonTrackerRunnerProxy::GetInstance();
}

PersonTracker::PersonTracker(const PersonTrackerOptions& options) {
  PersonTrackerRunnerProxy::AddReference();
  PersonTrackerRunnerProxy::GetInstance()->SetPersonTrackerOptions(options);
}

PersonTracker::PersonTracker(
    const PersonTrackerOptions& personTrackerOptions,
    const CameraOptions& cameraOptions) {
  PersonTrackerRunnerProxy::AddReference();
  PersonTrackerRunnerProxy::GetInstance()->SetPersonTrackerOptions(
      personTrackerOptions);
  PersonTrackerRunnerProxy::GetInstance()->SetCameraOptions(cameraOptions);
}

PersonTracker::PersonTracker(const PersonTracker& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_tracker.h
}

PersonTracker::~PersonTracker() {
  PersonTrackerRunnerProxy::RemoveReference();
}

PersonTracker& PersonTracker::operator = (const PersonTracker& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

v8::Local<v8::Promise> PersonTracker::getPersonTrackerOptions() {
  return PersonTrackerRunnerProxy::GetInstance()->GetPersonTrackerOptions();
}

v8::Local<v8::Promise> PersonTracker::setPersonTrackerOptions(
    const PersonTrackerOptions& personTrackerOptions) {
  return PersonTrackerRunnerProxy::GetInstance()->SetPersonTrackerOptions(
      personTrackerOptions);
}

v8::Local<v8::Promise> PersonTracker::getCameraOptions() {
  return PersonTrackerRunnerProxy::GetInstance()->GetCameraOptions();
}

v8::Local<v8::Promise> PersonTracker::setCameraOptions(
    const CameraOptions& cameraOptions) {
  return PersonTrackerRunnerProxy::GetInstance()->SetCameraOptions(
      cameraOptions);
}

v8::Local<v8::Promise> PersonTracker::getFrameData() {
  PromiseHelper resolver;
  auto promise = resolver.CreatePromise();
  auto data = PersonTrackerRunnerProxy::GetInstance()->GetFrameData();
  resolver.ResolvePromise(NanFrameData::NewInstance(data));
  return promise;
}

v8::Local<v8::Promise> PersonTracker::start() {
  return PersonTrackerRunnerProxy::GetInstance()->Start();
}

v8::Local<v8::Promise> PersonTracker::stop() {
  return PersonTrackerRunnerProxy::GetInstance()->Stop();
}

v8::Local<v8::Promise> PersonTracker::pause() {
  return PersonTrackerRunnerProxy::GetInstance()->Pause();
}

v8::Local<v8::Promise> PersonTracker::resume() {
  return PersonTrackerRunnerProxy::GetInstance()->Resume();
}

v8::Local<v8::Promise> PersonTracker::reset() {
  return PersonTrackerRunnerProxy::GetInstance()->Reset();
}

v8::Local<v8::Promise> PersonTracker::getPersonInfo(const int32_t& trackID) {
  return PersonTrackerRunnerProxy::GetInstance()->GetPersonInfo(trackID);
}
