// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/geometry/point2d.h"

Point2D::Point2D() {
}

Point2D::Point2D(const double& x, const double& y) : Point2DBase(x, y) {
}

Point2D::Point2D(const ArrayHelper& coords) {
  setCoords(coords);
}

Point2D::Point2D(const Point2D& rhs) : Point2DBase(rhs) {
}

void Point2D::setCoords(const ArrayHelper& coords) {
  if (coords.Length() > 0) {
    x_ = coords.GetFloat(0);
  }

  if (coords.Length() > 1) {
    y_ = coords.GetFloat(1);
  }

  NotifyChange();
}

ArrayHelper Point2D::getCoords() {
  ArrayHelper helper;
  helper.SetT(0, x_);
  helper.SetT(1, y_);
  return helper;
}

