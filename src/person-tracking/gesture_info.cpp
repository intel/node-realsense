// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gesture_info.h"

GestureInfo::GestureInfo()
    : isPointing_(false),
      has_pointing_info_(false) {
  // TODO(widl-nan): init your members
}

GestureInfo::GestureInfo(const GestureInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in gesture_info.h
}

GestureInfo::~GestureInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

GestureInfo& GestureInfo::operator = (const GestureInfo& rhs) {
  if (&rhs != this) {
    isPointing_ = rhs.isPointing_;
    has_pointing_info_ = rhs.has_pointing_info_;
    pointingInfo_ = rhs.pointingInfo_;
  }

  return *this;
}
