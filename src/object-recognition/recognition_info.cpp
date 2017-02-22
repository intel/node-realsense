// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "recognition_info.h"

RecognitionInfo::RecognitionInfo() {
  label_ = "<no-label>";
  probability_ = 0;
}

RecognitionInfo::RecognitionInfo(const std::string& label, double probability) {
  label_ = label;
  probability_ = probability;
}

RecognitionInfo::RecognitionInfo(const RecognitionInfo& rhs) {
  CopyFrom(rhs);
}

RecognitionInfo::~RecognitionInfo() {
}

RecognitionInfo& RecognitionInfo::operator = (const RecognitionInfo& rhs) {
  CopyFrom(rhs);
  return *this;
}

void RecognitionInfo::CopyFrom(const RecognitionInfo& rhs) {
  if (&rhs != this) {
    label_ = rhs.label_;
    probability_ = rhs.probability_;
  }
}

