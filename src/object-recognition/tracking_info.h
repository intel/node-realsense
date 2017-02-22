// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _TRACKING_INFO_H_
#define _TRACKING_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "rect2d.h"

#include "point3d.h"

class TrackingInfo {
 public:
  TrackingInfo();

  TrackingInfo(const Rect2D& roi, const Point3D& object_center);

  TrackingInfo(const TrackingInfo& rhs);

  ~TrackingInfo();

  TrackingInfo& operator = (const TrackingInfo& rhs);

 public:
  Rect2D* get_roi() const {
    return &this->roi_;
  }

  Point3D* get_objectCenter() const {
    return &this->object_center_;
  }

  void SetROI(const Rect2D& roi) {
    roi_ = roi;
  }

  void SetObjectCenter(const Point3D& object_center) {
    object_center_ = object_center;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  void CopyFrom(const TrackingInfo&);

 private:
  mutable Rect2D  roi_;
  mutable Point3D object_center_;
};

#endif  // _TRACKING_INFO_H_
