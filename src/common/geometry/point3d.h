// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_POINT3D_H_
#define _COMMON_GEOMETRY_POINT3D_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/geometry/point_base.h"

class Point3D : public Point3DBase {
 public:
  Point3D();

  explicit Point3D(const double& x, const double& y, const double& z);

  explicit Point3D(const ArrayHelper& coords);

  Point3D(const Point3D& rhs);

 public:
  void setCoords(const ArrayHelper& coords);
  using Point3DBase::setCoords;

  ArrayHelper getCoords();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }
};

#endif  // _COMMON_GEOMETRY_POINT3D_H_
