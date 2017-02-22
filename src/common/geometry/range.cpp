// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/geometry/range.h"

#include <cmath>
#include <sstream>

#include "common/geometry/float_equal.h"

Range::Range() {
  begin_ = end_ = 0;
}

Range::Range(const double& begin, const double& end) {
  begin_ = begin;
  end_ = end;
}

Range::Range(const Range& rhs) {
  begin_ = rhs.begin_;
  end_ = rhs.end_;
}

Range::~Range() {
}

Range& Range::operator = (const Range& rhs) {
  if (&rhs != this) {
    begin_ = rhs.begin_;
    end_ = rhs.end_;
  }
  return *this;
}

bool Range::contains(const double& value) {
  return value >= begin_ && value <= end_;
}

bool Range::properlyContains(const double& value) {
  return value > begin_ && value < end_;
}

bool Range::equal(const Range& range) {
  return equal(range.begin_, range.end_);
}

bool Range::equal(const double& begin, const double& end) {
  return double_equal(begin_, begin) && double_equal(end_, end);
}

bool Range::isEmpty() {
  return begin_ == end_;
}

bool Range::isValid() {
  return !std::isnan(begin_) && !std::isnan(end_) && end_ >= begin_;
}

std::string Range::ToString() const {
  std::stringstream ss;
  ss << "Range (" << get_min() << " - " << get_max() << ")";
  return ss.str();
}
