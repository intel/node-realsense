// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/intrinsics.h"

Intrinsics::Intrinsics() {
}

Intrinsics::Intrinsics(const Intrinsics& rhs) {
  CopyFrom(rhs);
}

Intrinsics::~Intrinsics() {
}

Intrinsics& Intrinsics::operator = (const Intrinsics& rhs) {
  CopyFrom(rhs);
  return *this;
}

void Intrinsics::CopyFrom(const Intrinsics& rhs) {
  if (this != &rhs) {
    width_ = rhs.width_;
    height_ = rhs.height_;
    principalPoint_ = rhs.principalPoint_;
    focalLength_ = rhs.focalLength_;
  }
}
