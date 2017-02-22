// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "bounding_box2d_info.h"

#include <memory>

BoundingBox2DInfo::BoundingBox2DInfo() {
  // TODO(widl-nan): init your members
}

BoundingBox2DInfo::BoundingBox2DInfo(const BoundingBox2DInfo& rhs) {
    rect_ = rhs.rect_;
    confidence_ = rhs.confidence_;
}

BoundingBox2DInfo::~BoundingBox2DInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

BoundingBox2DInfo& BoundingBox2DInfo::operator = (
  const BoundingBox2DInfo& rhs) {
  if (&rhs != this) {
    rect_ = rhs.rect_;
    confidence_ = rhs.confidence_;
  }

  return *this;
}
