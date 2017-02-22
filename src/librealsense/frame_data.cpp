// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "frame_data.h"

FrameData::FrameData() {
  data_.data = nullptr;
}

FrameData::FrameData(const FrameData& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in frame_data.h
}

FrameData::~FrameData() {
  free(data_.data);
}

FrameData& FrameData::operator = (const FrameData& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}
