// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_registration_data.h"

PersonRegistrationData::PersonRegistrationData() :
    trackID_(0),
    recognitionID_(0),
    descriptorID_(0) {
}

PersonRegistrationData::PersonRegistrationData(
    const PersonRegistrationData& rhs) {
  trackID_ = rhs.trackID_;
  recognitionID_ = rhs.recognitionID_;
  descriptorID_ = rhs.descriptorID_;
}

PersonRegistrationData::~PersonRegistrationData() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonRegistrationData& PersonRegistrationData::operator = (
    const PersonRegistrationData& rhs) {
  if (&rhs != this) {
    trackID_ = rhs.trackID_;
    recognitionID_ = rhs.recognitionID_;
    descriptorID_ = rhs.descriptorID_;
  }
  return *this;
}

