// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "face_landmark.h"

FaceLandmark::FaceLandmark() {
  // TODO(widl-nan): init your members
}

FaceLandmark::FaceLandmark(const FaceLandmark& rhs) {
  imageCoordinate_ = rhs.imageCoordinate_;
  worldCoordinate_ = rhs.worldCoordinate_;
}

FaceLandmark::~FaceLandmark() {
  // TODO(widl-nan): do cleanup if necessary
}

FaceLandmark& FaceLandmark::operator = (const FaceLandmark& rhs) {
  if (&rhs != this) {
    imageCoordinate_ = rhs.imageCoordinate_;
    worldCoordinate_ = rhs.worldCoordinate_;
  }

  return *this;
}

