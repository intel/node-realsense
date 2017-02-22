// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/geometry/size2d.h"

#include <cmath>
#include <sstream>

#include "common/geometry/float_equal.h"

Size2D::Size2D() {
  w_ = h_ = 0;
}

Size2D::Size2D(const double& width, const double& height) {
  w_ = width;
  h_ = height;
}

Size2D::Size2D(const Size2D& rhs) {
  w_ = rhs.w_;
  h_ = rhs.h_;
}

Size2D::~Size2D() {
}

Size2D& Size2D::operator = (const Size2D& rhs) {
  if (&rhs != this) {
    w_ = rhs.w_;
    h_ = rhs.h_;
  }
  return *this;
}

bool Size2D::equal(const Size2D& size) {
  return equal(size.w_, size.h_);
}

bool Size2D::equal(const double& width, const double& height) {
  return double_equal(w_, width) && double_equal(h_, height);
}

bool Size2D::isEmpty() {
  return w_ == 0 || h_ == 0;
}

bool Size2D::isValid() {
  return !std::isnan(w_) && !std::isnan(h_) && w_ >= 0 && h_ >= 0;
}

std::string Size2D::ToString() const {
  std::stringstream ss;
  ss << "Size2D (" << get_width() << ", " << get_height() << ")";
  return ss.str();
}
