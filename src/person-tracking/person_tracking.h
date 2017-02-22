// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_TRACKING_H_
#define _PERSON_TRACKING_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "worker/person_tracker_adapter.h"

class PersonTracking {
 public:
  PersonTracking();

  PersonTracking(const PersonTracking& rhs);

  ~PersonTracking();

  PersonTracking& operator = (const PersonTracking& rhs);

 public:
  v8::Handle<v8::Promise> startTrackingPerson(const int32_t& trackID);

  v8::Handle<v8::Promise> stopTrackingPerson(const int32_t& trackID);

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  PersonTrackerAdapter* tracker;
};

#endif  // _PERSON_TRACKING_H_
