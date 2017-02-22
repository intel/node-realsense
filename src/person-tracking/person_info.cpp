// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_info.h"

PersonInfo::PersonInfo()
    : has_skeleton_(false),
      has_track_(false),
      has_gesture_(false),
      has_face_(false),
      has_pose_(false) {
  // TODO(widl-nan): init your members
}

PersonInfo::PersonInfo(const PersonInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_info.h
}

PersonInfo::~PersonInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonInfo& PersonInfo::operator = (const PersonInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

