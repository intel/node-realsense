// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_REGISTRATION_DATA_H_
#define _PERSON_REGISTRATION_DATA_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class PersonRegistrationData {
 public:
  PersonRegistrationData();

  PersonRegistrationData(const PersonRegistrationData& rhs);

  ~PersonRegistrationData();

  PersonRegistrationData& operator = (const PersonRegistrationData& rhs);

 public:
  int32_t get_trackID() const {
    return this->trackID_;
  }

  int32_t get_recognitionID() const {
    return this->recognitionID_;
  }

  int32_t get_descriptorID() const {
    return this->descriptorID_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t trackID_;

  int32_t recognitionID_;

  int32_t descriptorID_;
  friend class PersonTrackerAdapter;
};

#endif  // _PERSON_REGISTRATION_DATA_H_
