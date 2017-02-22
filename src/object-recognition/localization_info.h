// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _LOCALIZATION_INFO_H_
#define _LOCALIZATION_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

#include "rect2d.h"
#include "point3d.h"

class LocalizationInfo {
 public:
  LocalizationInfo();

  LocalizationInfo(const std::string& label, double probability,
    const Rect2D& roi, const Point3D& objectCenter);

  LocalizationInfo(const LocalizationInfo& rhs);

  ~LocalizationInfo();

  LocalizationInfo& operator = (const LocalizationInfo& rhs);

  void CopyFrom(const LocalizationInfo& rhs);

 public:
  std::string get_label() const {
    return this->label_;
  }

  double get_probability() const {
    return this->probability_;
  }

  Rect2D* get_roi() const {
    return &this->roi_;
  }

  Point3D* get_objectCenter() const {
    return &this->objectCenter_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::string label_;

  double probability_;

  mutable Rect2D roi_;

  mutable Point3D objectCenter_;
};

#endif  // _LOCALIZATION_INFO_H_
