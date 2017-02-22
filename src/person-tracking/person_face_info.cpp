// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_face_info.h"

PersonFaceInfo::PersonFaceInfo()
    : has_landmark_(false),
      has_head_pose_(false),
      has_expression_(false) {
}

PersonFaceInfo::PersonFaceInfo(const PersonFaceInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_face_info.h
}

PersonFaceInfo::~PersonFaceInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonFaceInfo& PersonFaceInfo::operator = (const PersonFaceInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}
