// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_tracking_result.h"

PersonTrackingResult::PersonTrackingResult() {
  // TODO(widl-nan): init your members
}

PersonTrackingResult::PersonTrackingResult(const PersonTrackingResult& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_tracking_result.h
}

PersonTrackingResult::~PersonTrackingResult() {
  persons_.FreeArray();
}

PersonTrackingResult& PersonTrackingResult::operator = (
  const PersonTrackingResult& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

