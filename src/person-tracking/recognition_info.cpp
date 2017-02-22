// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "recognition_info.h"

RecognitionInfo::RecognitionInfo() {
  // TODO(widl-nan): init your members
}

RecognitionInfo::RecognitionInfo(const RecognitionInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in recognition_info.h
}

RecognitionInfo::~RecognitionInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

RecognitionInfo& RecognitionInfo::operator = (const RecognitionInfo& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

