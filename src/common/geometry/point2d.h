// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_POINT2D_H_
#define _COMMON_GEOMETRY_POINT2D_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/geometry/point_base.h"

class Point2D : public Point2DBase {
 public:
  Point2D();

  explicit Point2D(const double& x, const double& y);

  explicit Point2D(const ArrayHelper& coords);

  Point2D(const Point2D& rhs);

  ~Point2D() {}

 public:
  using Point2DBase::setCoords;
  void setCoords(const ArrayHelper& coords);

  ArrayHelper getCoords();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }
};

#endif  // _COMMON_GEOMETRY_POINT2D_H_
