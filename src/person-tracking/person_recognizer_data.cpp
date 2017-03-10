// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_recognizer_data.h"

PersonRecognizerData::PersonRecognizerData() :
    recognized_(false),
    trackID_(0),
    recognitionID_(0),
    similarityScore_(0) {
}

PersonRecognizerData::PersonRecognizerData(const PersonRecognizerData& rhs) {
  recognized_ = rhs.recognized_;
  recognitionID_ = rhs.recognitionID_;
  trackID_ = rhs.trackID_;
  similarityScore_ = rhs.similarityScore_;
}

PersonRecognizerData::~PersonRecognizerData() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonRecognizerData& PersonRecognizerData::operator = (
    const PersonRecognizerData& rhs) {
  if (&rhs != this) {
    recognized_ = rhs.recognized_;
    recognitionID_ = rhs.recognitionID_;
    trackID_ = rhs.trackID_;
    similarityScore_ = rhs.similarityScore_;
  }
  return *this;
}

