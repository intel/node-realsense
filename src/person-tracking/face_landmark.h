// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _FACE_LANDMARK_H_
#define _FACE_LANDMARK_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "common/geometry/point3d.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class FaceLandmark {
 public:
  FaceLandmark();

  FaceLandmark(const FaceLandmark& rhs);

  ~FaceLandmark();

  FaceLandmark& operator = (const FaceLandmark& rhs);

 public:
  Point3D* get_imageCoordinate() const {
    return &this->imageCoordinate_;
  }

  Point3D* get_worldCoordinate() const {
    return &this->worldCoordinate_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Point3D imageCoordinate_;

  mutable Point3D worldCoordinate_;
  friend class PersonTrackerAdapter;
};

#endif  // _FACE_LANDMARK_H_
