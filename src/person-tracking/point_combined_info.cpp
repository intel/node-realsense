// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "point_combined_info.h"

#include <memory>

PointCombinedInfo::PointCombinedInfo()
    : worldConfidence_(0),
      imageConfidence_(0) {
  // TODO(widl-nan): init your members
}

PointCombinedInfo::PointCombinedInfo(const PointCombinedInfo& rhs) {
  worldCoordinate_ = rhs.worldCoordinate_;
  imageCoordinate_ = rhs.imageCoordinate_;
  worldConfidence_ = rhs.worldConfidence_;
  imageConfidence_ = rhs.imageConfidence_;
}

PointCombinedInfo::~PointCombinedInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

PointCombinedInfo& PointCombinedInfo::operator = (
  const PointCombinedInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
    // memcpy((void*)this, (void*)&rhs, sizeof(PointCombinedInfo));   // NOLINT
    worldCoordinate_ = rhs.worldCoordinate_;
    imageCoordinate_ = rhs.imageCoordinate_;
    worldConfidence_ = rhs.worldConfidence_;
    imageConfidence_ = rhs.imageConfidence_;
  }

  return *this;
}
