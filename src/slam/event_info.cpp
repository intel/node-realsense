// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "event_info.h"

EventInfo::EventInfo() {
  // TODO(widl-nan): init your members
}

EventInfo::EventInfo(const EventInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in event_info.h
}

EventInfo::~EventInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

EventInfo& EventInfo::operator = (const EventInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}
