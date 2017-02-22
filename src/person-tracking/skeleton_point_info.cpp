// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "skeleton_point_info.h"

SkeletonPointInfo::SkeletonPointInfo() {
  // TODO(widl-nan): init your members
}

SkeletonPointInfo::SkeletonPointInfo(const SkeletonPointInfo& rhs) {
  jointType_ = rhs.jointType_;
  worldCoordinate_ = rhs.worldCoordinate_;
  imageCoordinate_ = rhs.imageCoordinate_;
  worldConfidence_ = rhs.worldConfidence_;
  imageConfidence_ = rhs.imageConfidence_;
}

SkeletonPointInfo::~SkeletonPointInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

SkeletonPointInfo& SkeletonPointInfo::operator = (
  const SkeletonPointInfo& rhs) {
  if (&rhs != this) {
    jointType_ = rhs.jointType_;
    worldCoordinate_ = rhs.worldCoordinate_;
    imageCoordinate_ = rhs.imageCoordinate_;
    worldConfidence_ = rhs.worldConfidence_;
    imageConfidence_ = rhs.imageConfidence_;
  }

  return *this;
}
