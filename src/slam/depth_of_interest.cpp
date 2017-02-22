// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "depth_of_interest.h"

DepthOfInterest::DepthOfInterest() {
  // TODO(widl-nan): init your members
}

DepthOfInterest::DepthOfInterest(const DepthOfInterest& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in depth_of_interest.h
}

DepthOfInterest::~DepthOfInterest() {
  // TODO(widl-nan): do cleanup if necessary
}

DepthOfInterest& DepthOfInterest::operator = (const DepthOfInterest& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

