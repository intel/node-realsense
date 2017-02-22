// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _LYING_POSE_INFO_H_
#define _LYING_POSE_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "bounding_box2d_info.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"
#include "point_combined_info.h"

class LyingPoseInfo {
 public:
  LyingPoseInfo();

  LyingPoseInfo(const LyingPoseInfo& rhs);

  ~LyingPoseInfo();

  LyingPoseInfo& operator = (const LyingPoseInfo& rhs);

 public:
  PointCombinedInfo* get_position() const {
    return &this->position_;
  }

  void set_position(const PointCombinedInfo& new_value) {
    this->position_ = new_value;
  }

  BoundingBox2DInfo* get_boundingBox() const {
    return &this->boundingBox_;
  }

  void set_boundingBox(const BoundingBox2DInfo& new_value) {
    this->boundingBox_ = new_value;
  }

  std::string get_result() const {
    return this->result_;
  }

  void set_result(const std::string& new_value) {
    this->result_ = new_value;
  }

  int32_t get_confidence() const {
    return this->confidence_;
  }

  void set_confidence(const int32_t& new_value) {
    this->confidence_ = new_value;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable PointCombinedInfo position_;

  mutable BoundingBox2DInfo boundingBox_;

  std::string result_;

  int32_t confidence_;
};

#endif  // _LYING_POSE_INFO_H_
