// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_SIZE2D_H_
#define _COMMON_GEOMETRY_SIZE2D_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/geometry/geometry_change_notify.h"

class Size2D : public GeometryChangeNotifyBase {
 public:
  Size2D();

  explicit Size2D(const double& width, const double& height);

  Size2D(const Size2D& rhs);

  ~Size2D();

  Size2D& operator = (const Size2D& rhs);

 public:
  std::string ToString() const;

  double get_width() const {
    return this->w_;
  }

  void set_width(const double& new_value) {
    this->w_ = new_value;
    NotifyChange();
  }

  double get_height() const {
    return this->h_;
  }

  void set_height(const double& new_value) {
    this->h_ = new_value;
    NotifyChange();
  }

  double get_cx() const {
    return this->w_;
  }

  void set_cx(const double& new_value) {
    this->w_ = new_value;
    NotifyChange();
  }

  double get_cy() const {
    return this->h_;
  }

  void set_cy(const double& new_value) {
    this->h_ = new_value;
    NotifyChange();
  }

  void quietlySetSize(double width, double height) {
    w_ = width;
    h_ = height;
  }

  bool equal(const Size2D& size);

  bool equal(const double& width, const double& height);

  bool isEmpty();

  bool isValid();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  double w_;
  double h_;
};

#endif  // _COMMON_GEOMETRY_SIZE2D_H_
