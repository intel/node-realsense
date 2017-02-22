// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/geometry/point3d.h"

Point3D::Point3D() {
}

Point3D::Point3D(const double& x, const double& y, const double& z)
    : Point3DBase(x, y, z) {
}

Point3D::Point3D(const ArrayHelper& coords) {
  setCoords(coords);
}

Point3D::Point3D(const Point3D& rhs) : Point3DBase(rhs) {
}

void Point3D::setCoords(const ArrayHelper& coords) {
  if (coords.Length() > 0) {
    x_ = coords.GetFloat(0);
  }

  if (coords.Length() > 1) {
    y_ = coords.GetFloat(1);
  }

  if (coords.Length() > 2) {
    z_ = coords.GetFloat(2);
  }
}

ArrayHelper Point3D::getCoords() {
  ArrayHelper helper;
  helper.SetT(0, x_);
  helper.SetT(1, y_);
  helper.SetT(2, z_);
  return helper;
}
