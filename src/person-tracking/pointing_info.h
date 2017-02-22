// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POINTING_INFO_H_
#define _POINTING_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "pointing_data2d.h"
#include "pointing_data3d.h"

class PointingInfo {
 public:
  PointingInfo();

  PointingInfo(const PointingInfo& rhs);

  ~PointingInfo();

  PointingInfo& operator = (const PointingInfo& rhs);

 public:
  PointingData3D* get_worldPointingData() const {
    return &this->worldPointingData_;
  }

  PointingData2D* get_colorPointingData() const {
    return &this->colorPointingData_;
  }

  int32_t get_confidence() const {
    return this->confidence_;
  }

  int32_t get_gestureStartTimeStamp() const {
    return this->gestureStartTimeStamp_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable PointingData3D worldPointingData_;

  mutable PointingData2D colorPointingData_;

  int32_t confidence_;

  int32_t gestureStartTimeStamp_;
  friend class PersonTrackerAdapter;
};

#endif  // _POINTING_INFO_H_
