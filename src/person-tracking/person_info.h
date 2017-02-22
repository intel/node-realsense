// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_INFO_H_
#define _PERSON_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "gesture_info.h"
#include "person_face_info.h"
#include "pose_info.h"
#include "skeleton_info.h"
#include "tracking_info.h"

class PersonInfo {
 public:
  PersonInfo();

  PersonInfo(const PersonInfo& rhs);

  ~PersonInfo();

  PersonInfo& operator = (const PersonInfo& rhs);

 public:
  SkeletonInfo* get_skeletonInfo() const {
    return has_skeleton_ ? &this->skeletonInfo_ : nullptr;
  }

  TrackingInfo* get_trackInfo() const {
    return has_track_ ? &this->trackInfo_ : nullptr;
  }

  GestureInfo* get_gestureInfo() const {
    return has_gesture_ ? &this->gestureInfo_ : nullptr;
  }

  PersonFaceInfo* get_faceInfo() const {
    return has_face_ ? &this->faceInfo_ : nullptr;
  }

  PoseInfo* get_poseInfo() const {
    return has_pose_ ? &this->poseInfo_ : nullptr;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable SkeletonInfo skeletonInfo_;

  mutable TrackingInfo trackInfo_;

  mutable GestureInfo gestureInfo_;

  mutable PersonFaceInfo faceInfo_;

  mutable PoseInfo poseInfo_;

  bool has_skeleton_;
  bool has_track_;
  bool has_gesture_;
  bool has_face_;
  bool has_pose_;
  friend class PersonDataInternal;
};

#endif  // _PERSON_INFO_H_
