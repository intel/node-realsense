// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_RANGE_H_
#define _COMMON_GEOMETRY_RANGE_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/geometry/geometry_change_notify.h"

class Range : public GeometryChangeNotifyBase {
 public:
  Range();

  explicit Range(const double& begin, const double& end);

  Range(const Range& rhs);

  ~Range();

  Range& operator = (const Range& rhs);

 public:
  std::string ToString() const;

  double get_begin() const {
    return this->begin_;
  }

  void set_begin(const double& new_value) {
    this->begin_ = new_value;
    NotifyChange();
  }

  double get_end() const {
    return this->end_;
  }

  void set_end(const double& new_value) {
    this->end_ = new_value;
    NotifyChange();
  }

  double get_min() const {
    return this->begin_;
  }

  void set_min(const double& new_value) {
    this->begin_ = new_value;
    NotifyChange();
  }

  double get_max() const {
    return this->end_;
  }

  void set_max(const double& new_value) {
    this->end_ = new_value;
    NotifyChange();
  }

  double get_length() const {
    return this->end_ - this->begin_;
  }

  void quietlySetRange(double begin, double end) {
    begin_ = begin;
    end_ = end;
  }

  bool contains(const double& value);

  bool properlyContains(const double& value);

  bool equal(const Range& range);

  bool equal(const double& begin, const double& end);

  bool isEmpty();

  bool isValid();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  double begin_;
  double end_;
};

#endif  // _COMMON_GEOMETRY_RANGE_H_
