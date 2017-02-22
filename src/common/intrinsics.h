// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_INTRINSICS_H_
#define _COMMON_INTRINSICS_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "point2d.h"

class Intrinsics {
 public:
  Intrinsics();

  Intrinsics(const Intrinsics& rhs);

  ~Intrinsics();

  Intrinsics& operator = (const Intrinsics& rhs);

 public:
  int32_t get_width() const {
    return this->width_;
  }

  void set_width(const int32_t& new_value) {
    this->width_ = new_value;
  }

  int32_t get_height() const {
    return this->height_;
  }

  void set_height(const int32_t& new_value) {
    this->height_ = new_value;
  }

  Point2D* get_principalPoint() const {
    return &this->principalPoint_;
  }

  void set_principalPoint(const Point2D& new_value) {
    this->principalPoint_ = new_value;
  }

  Point2D* get_focalLength() const {
    return &this->focalLength_;
  }

  void set_focalLength(const Point2D& new_value) {
    this->focalLength_ = new_value;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  void CopyFrom(const Intrinsics& rhs);

 private:
  int32_t width_;

  int32_t height_;

  mutable Point2D principalPoint_;

  mutable Point2D focalLength_;
};

#endif  // _COMMON_INTRINSICS_H_
