// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _TRACKING_RESULT_H_
#define _TRACKING_RESULT_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "frame_data.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class TrackingResult {
 public:
  TrackingResult();

  TrackingResult(const TrackingResult& rhs) = delete;

  ~TrackingResult();

  TrackingResult& operator = (const TrackingResult& rhs);

 public:
  std::string get_accuracy() const { return this->accuracy_; }

  void set_accuracy(const std::string& accuracy) { accuracy_ = accuracy; }

  ArrayHelper get_cameraPose() const { return this->camera_pose_; }

  void set_camera_pose(ArrayHelper pose) { camera_pose_ = pose; }

  FrameData* get_frameData() const { return &this->frame_data_; }

  void set_frame_data(FrameData* info) { frame_data_ = *info; }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::string accuracy_;

  ArrayHelperStorage camera_pose_;

  mutable FrameData frame_data_;
};

#endif  // _TRACKING_RESULT_H_
