// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _GESTURE_INFO_H_
#define _GESTURE_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"
#include "pointing_info.h"

class GestureInfo {
 public:
  GestureInfo();

  GestureInfo(const GestureInfo& rhs);

  ~GestureInfo();

  GestureInfo& operator = (const GestureInfo& rhs);

 public:
  bool get_isPointing() const {
    return this->isPointing_;
  }

  PointingInfo* get_pointingInfo() const {
    return has_pointing_info_ ? &this->pointingInfo_ : nullptr;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  bool isPointing_;

  mutable PointingInfo pointingInfo_;
  bool has_pointing_info_;
  friend class PersonTrackerAdapter;
};

#endif  // _GESTURE_INFO_H_
