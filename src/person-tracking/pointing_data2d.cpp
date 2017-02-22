// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "pointing_data2d.h"

PointingData2D::PointingData2D() {
  // TODO(widl-nan): init your members
}

PointingData2D::PointingData2D(const PointingData2D& rhs) {
  origin_ = rhs.origin_;
  direction_ = rhs.direction_;
}

PointingData2D::~PointingData2D() {
  // TODO(widl-nan): do cleanup if necessary
}

PointingData2D& PointingData2D::operator = (const PointingData2D& rhs) {
  if (&rhs != this) {
    origin_ = rhs.origin_;
    direction_ = rhs.direction_;
  }
  return *this;
}

