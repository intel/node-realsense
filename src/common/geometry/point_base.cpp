// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "point_base.h"

double Point2DBase::distance(const Point2DBase& p1, const Point2DBase& p2) {
  return distance(p1.x_, p1.y_, p2.x_, p2.y_);
}

double Point2DBase::distance(const double& x1, const double& y1,
                             const double& x2, const double& y2) {
  return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

double Point3DBase::distance(const Point3DBase& p1, const Point3DBase& p2) {
  return distance(p1.x_, p1.y_, p1.z_, p2.x_, p2.y_, p2.z_);
}

double Point3DBase::distance(const double& x1,
                             const double& y1,
                             const double& z1,
                             const double& x2,
                             const double& y2,
                             const double& z2) {
  auto dx = x1 - x2;
  auto dy = y1 - y2;
  auto dz = z1 - z2;

  return sqrt(dx*dx + dy*dy + dz*dz);
}
