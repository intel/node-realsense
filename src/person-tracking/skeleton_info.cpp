// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "skeleton_info.h"

SkeletonInfo::SkeletonInfo() {
  // TODO(widl-nan): init your members
}

SkeletonInfo::SkeletonInfo(const SkeletonInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in skeleton_info.h
}

SkeletonInfo::~SkeletonInfo() {
  skeletonJoints_.FreeArray();
}

SkeletonInfo& SkeletonInfo::operator = (const SkeletonInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}
