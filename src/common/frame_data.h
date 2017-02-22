// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_FRAME_DATA_H_
#define _COMMON_FRAME_DATA_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/image.h"
#include "common/motion_sample.h"

class FrameData {
 public:
  FrameData();

  FrameData(const FrameData& rhs) = delete;

  ~FrameData();

  FrameData& operator = (const FrameData& rhs);

 public:
  Image* get_fisheye() const {
    return has_fisheye_ ? &this->fisheye_ : NULL;
  }

  void set_fisheye(Image* value) {
    if (value) {
      fisheye_.Assign(value);
      has_fisheye_ = true;
    }
  }

  Image* get_color() const {
    return has_color_ ? &this->color_ : NULL;
  }

  void set_color(Image* value) {
    if (value) {
      color_.Assign(value);
      has_color_ = true;
    }
  }

  Image* get_depth() const {
    return has_depth_ ? &this->depth_ : NULL;
  }

  void set_depth(Image* value) {
    if (value) {
      depth_.Assign(value);
      has_depth_ = true;
    }
  }

  MotionSample* get_gyro() const {
    return has_gyro_ ? &this->gyro_ : NULL;
  }

  void set_gyro(MotionSample* value) {
    if (value) {
      gyro_ = *value;
      has_gyro_ = true;
    }
  }

  MotionSample* get_accel() const {
    return has_accel_ ? &this->accel_ : NULL;
  }

  void set_accel(MotionSample* value) {
    if (value) {
      accel_ = *value;
      has_accel_ = true;
    }
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Image fisheye_;

  mutable Image color_;

  mutable Image depth_;

  mutable MotionSample gyro_;

  mutable MotionSample accel_;
  bool has_fisheye_;
  bool has_color_;
  bool has_depth_;
  bool has_gyro_;
  bool has_accel_;
};

#endif  // _COMMON_FRAME_DATA_H_
