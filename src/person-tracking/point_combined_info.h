// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POINT_COMBINED_INFO_H_
#define _POINT_COMBINED_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "common/geometry/point2d.h"
#include "common/geometry/point3d.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class PointCombinedInfo {
 public:
  PointCombinedInfo();

  PointCombinedInfo(const PointCombinedInfo& rhs);

  ~PointCombinedInfo();

  PointCombinedInfo& operator = (const PointCombinedInfo& rhs);

 public:
  Point3D* get_worldCoordinate() const {
    return &this->worldCoordinate_;
  }

  Point2D* get_imageCoordinate() const {
    return &this->imageCoordinate_;
  }

  int32_t get_worldConfidence() const {
    return this->worldConfidence_;
  }

  int32_t get_imageConfidence() const {
    return this->imageConfidence_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Point3D worldCoordinate_;

  mutable Point2D imageCoordinate_;

  int32_t worldConfidence_;

  int32_t imageConfidence_;
  friend class PersonTrackerAdapter;
};

#endif  // _POINT_COMBINED_INFO_H_
