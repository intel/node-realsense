// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "tracking_event.h"

TrackingEvent::TrackingEvent() {
  // TODO(widl-nan): init your members
}

TrackingEvent::TrackingEvent(const TrackingEvent& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in tracking_event.h
}

TrackingEvent::~TrackingEvent() {
  // TODO(widl-nan): do cleanup if necessary
}

TrackingEvent& TrackingEvent::operator = (const TrackingEvent& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}
