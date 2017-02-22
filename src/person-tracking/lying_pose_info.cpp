// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "lying_pose_info.h"

LyingPoseInfo::LyingPoseInfo() {
  // TODO(widl-nan): init your members
}

LyingPoseInfo::LyingPoseInfo(const LyingPoseInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in lying_pose_info.h
}

LyingPoseInfo::~LyingPoseInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

LyingPoseInfo& LyingPoseInfo::operator = (const LyingPoseInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

