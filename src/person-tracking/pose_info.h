// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _POSE_INFO_H_
#define _POSE_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class PoseInfo {
 public:
  PoseInfo();

  PoseInfo(const PoseInfo& rhs);

  ~PoseInfo();

  PoseInfo& operator = (const PoseInfo& rhs);

 public:
  std::string get_position() const {
    return this->position_;
  }

  int32_t get_confidence() const {
    return this->confidence_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::string position_;

  int32_t confidence_;
  friend class PersonTrackerAdapter;
};

#endif  // _POSE_INFO_H_
