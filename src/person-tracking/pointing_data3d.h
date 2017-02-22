// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POINTING_DATA3D_H_
#define _POINTING_DATA3D_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "common/geometry/point3d.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class PointingData3D {
 public:
  PointingData3D();

  PointingData3D(const PointingData3D& rhs);

  ~PointingData3D();

  PointingData3D& operator = (const PointingData3D& rhs);

 public:
  Point3D* get_origin() const {
    return &this->origin_;
  }

  Point3D* get_direction() const {
    return &this->direction_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Point3D origin_;

  mutable Point3D direction_;
  friend class PersonTrackerAdapter;
};

#endif  // _POINTING_DATA3D_H_
