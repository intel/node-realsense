// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "pointing_data3d.h"

PointingData3D::PointingData3D() {
  // TODO(widl-nan): init your members
}

PointingData3D::PointingData3D(const PointingData3D& rhs) {
  origin_ = rhs.origin_;
  direction_ = rhs.direction_;
}

PointingData3D::~PointingData3D() {
  // TODO(widl-nan): do cleanup if necessary
}

PointingData3D& PointingData3D::operator = (const PointingData3D& rhs) {
  if (&rhs != this) {
    origin_ = rhs.origin_;
    direction_ = rhs.direction_;
  }
  return *this;
}

