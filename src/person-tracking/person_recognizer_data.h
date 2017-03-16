// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_RECOGNIZER_DATA_H_
#define _PERSON_RECOGNIZER_DATA_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class PersonRecognizerData {
 public:
  PersonRecognizerData();

  PersonRecognizerData(const PersonRecognizerData& rhs);

  ~PersonRecognizerData();

  PersonRecognizerData& operator = (const PersonRecognizerData& rhs);

 public:
  bool get_recognized() const {
    return this->recognized_;
  }

  int32_t get_trackID() const {
    return this->trackID_;
  }

  int32_t get_recognitionID() const {
    return this->recognitionID_;
  }

  int32_t get_similarityScore() const {
    return this->similarityScore_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  bool recognized_;

  int32_t trackID_;

  int32_t recognitionID_;

  int32_t similarityScore_;
  friend class PersonTrackerAdapter;
};

#endif  // _PERSON_RECOGNIZER_DATA_H_
