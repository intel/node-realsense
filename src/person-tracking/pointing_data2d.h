// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POINTING_DATA2D_H_
#define _POINTING_DATA2D_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "common/geometry/point2d.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class PointingData2D {
 public:
  PointingData2D();

  PointingData2D(const PointingData2D& rhs);

  ~PointingData2D();

  PointingData2D& operator = (const PointingData2D& rhs);

 public:
  Point2D* get_origin() const {
    return &this->origin_;
  }

  Point2D* get_direction() const {
    return &this->direction_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Point2D origin_;

  mutable Point2D direction_;
  friend class PersonTrackerAdapter;
};

#endif  // _POINTING_DATA2D_H_
