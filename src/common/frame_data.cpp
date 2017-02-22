// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/frame_data.h"

FrameData::FrameData()
    : has_fisheye_(false),
      has_color_(false),
      has_depth_(false),
      has_gyro_(false),
      has_accel_(false) {
  // TODO(widl-nan): init your members
}

FrameData::~FrameData() {
  // TODO(widl-nan): do cleanup if necessary
}

FrameData& FrameData::operator = (const FrameData& rhs) {
  if (&rhs != this) {
    set_fisheye(rhs.get_fisheye());
    set_color(rhs.get_color());
    set_depth(rhs.get_depth());
    set_gyro(rhs.get_gyro());
    set_accel(rhs.get_accel());
  }
  return *this;
}
