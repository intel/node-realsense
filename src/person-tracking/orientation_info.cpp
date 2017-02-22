// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "orientation_info.h"

OrientationInfo::OrientationInfo() {
  // TODO(widl-nan): init your members
}

OrientationInfo::~OrientationInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

OrientationInfo& OrientationInfo::operator = (const OrientationInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
    orientation_ = rhs.orientation_;
    confidence_ = rhs.confidence_;
  }

  return *this;
}

