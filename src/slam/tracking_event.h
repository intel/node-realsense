// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _TRACKING_EVENT_H_
#define _TRACKING_EVENT_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "event_info.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class TrackingEvent {
 public:
  TrackingEvent();

  TrackingEvent(const TrackingEvent& rhs);

  ~TrackingEvent();

  TrackingEvent& operator = (const TrackingEvent& rhs);

 public:
  std::string get_eventName() const {
    return this->eventName_;
  }

  EventInfo* get_data() const {
    return &this->data_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::string eventName_;

  mutable EventInfo data_;
};

#endif  // _TRACKING_EVENT_H_
