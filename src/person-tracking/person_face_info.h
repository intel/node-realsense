// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_FACE_INFO_H_
#define _PERSON_FACE_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "expression_info_collection.h"
#include "face_landmark_info.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"
#include "pose_euler_angles.h"

class PersonFaceInfo {
 public:
  PersonFaceInfo();

  PersonFaceInfo(const PersonFaceInfo& rhs);

  ~PersonFaceInfo();

  PersonFaceInfo& operator = (const PersonFaceInfo& rhs);

 public:
  FaceLandmarkInfo* get_landmarkInfo() const {
    return has_landmark_ ? &this->landmarkInfo_ : nullptr;
  }

  PoseEulerAngles* get_headPose() const {
    return has_head_pose_ ? &this->headPose_ : nullptr;
  }

  ExpressionInfoCollection* get_expressionInfo() const {
    return has_expression_ ? &this->expressionInfo_ : nullptr;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable FaceLandmarkInfo landmarkInfo_;

  mutable PoseEulerAngles headPose_;

  mutable ExpressionInfoCollection expressionInfo_;
  bool has_landmark_;
  bool has_head_pose_;
  bool has_expression_;
  friend class PersonTrackerAdapter;
  friend class PersonDataInternal;
};

#endif  // _PERSON_FACE_INFO_H_
