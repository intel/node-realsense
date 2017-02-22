// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "pose_info.h"

PoseInfo::PoseInfo() {
  // TODO(widl-nan): init your members
}

PoseInfo::PoseInfo(const PoseInfo& rhs) {
  position_ = rhs.position_;
  confidence_ = rhs.confidence_;
}

PoseInfo::~PoseInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

PoseInfo& PoseInfo::operator = (const PoseInfo& rhs) {
  if (&rhs != this) {
    position_ = rhs.position_;
    confidence_ = rhs.confidence_;
  }

  return *this;
}

