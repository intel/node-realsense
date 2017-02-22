// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "face_landmark_info.h"

FaceLandmarkInfo::FaceLandmarkInfo() {
  // TODO(widl-nan): init your members
}

FaceLandmarkInfo::FaceLandmarkInfo(const FaceLandmarkInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in face_landmark_info.h
}

FaceLandmarkInfo::~FaceLandmarkInfo() {
  landmarks_.FreeArray();
}

FaceLandmarkInfo& FaceLandmarkInfo::operator = (const FaceLandmarkInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

