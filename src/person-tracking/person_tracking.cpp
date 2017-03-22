// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <memory>

#include "person_tracking.h"
#include "worker/person_tracker_runner_proxy.h"

PersonTracking::PersonTracking() {}

PersonTracking::PersonTracking(const PersonTracking& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_tracking.h
}

PersonTracking::~PersonTracking() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonTracking& PersonTracking::operator = (const PersonTracking& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

v8::Handle<v8::Promise> PersonTracking::startTrackingPerson(
    const int32_t& trackID) {
  return PersonTrackerRunnerProxy::GetInstance()->StartTrackingPerson(trackID);
}

v8::Handle<v8::Promise> PersonTracking::stopTrackingPerson(
    const int32_t& trackID) {
  return PersonTrackerRunnerProxy::GetInstance()->StopTrackingPerson(trackID);
}

v8::Handle<v8::Promise> PersonTracking::resetTracking() {
  return PersonTrackerRunnerProxy::GetInstance()->ResetTracking();
}
