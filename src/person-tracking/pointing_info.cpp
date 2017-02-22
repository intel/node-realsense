// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "pointing_info.h"

#include <memory>

PointingInfo::PointingInfo() {
  // TODO(widl-nan): init your members
}

PointingInfo::PointingInfo(const PointingInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in pointing_info.h
}

PointingInfo::~PointingInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

PointingInfo& PointingInfo::operator = (const PointingInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
    worldPointingData_ = rhs.worldPointingData_;
    colorPointingData_ = rhs.colorPointingData_;
    confidence_ = rhs.confidence_;
    gestureStartTimeStamp_ = rhs.gestureStartTimeStamp_;
  }

  return *this;
}
