// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POSE_EULER_ANGLES_H_
#define _POSE_EULER_ANGLES_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class PoseEulerAngles {
 public:
  PoseEulerAngles();

  ~PoseEulerAngles();

  PoseEulerAngles& operator = (const PoseEulerAngles& rhs);

 public:
  double get_yaw() const {
    return this->yaw_;
  }

  double get_pitch() const {
    return this->pitch_;
  }

  double get_roll() const {
    return this->roll_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  double yaw_;

  double pitch_;

  double roll_;
  friend class PersonTrackerAdapter;
};

#endif  // _POSE_EULER_ANGLES_H_
