// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "pose_euler_angles.h"

#include <memory>

PoseEulerAngles::PoseEulerAngles() : yaw_(0), pitch_(0), roll_(0) {}

PoseEulerAngles::~PoseEulerAngles() {
  // TODO(widl-nan): do cleanup if necessary
}

PoseEulerAngles& PoseEulerAngles::operator = (
  const PoseEulerAngles& rhs) {
  if (&rhs != this) {
    yaw_ = rhs.yaw_;
    pitch_ = rhs.pitch_;
    roll_ = rhs.roll_;
  }
  return *this;
}
