// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _RECOGNITION_INFO_H_
#define _RECOGNITION_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class RecognitionInfo {
 public:
  RecognitionInfo();

  RecognitionInfo(const RecognitionInfo& rhs);

  ~RecognitionInfo();

  RecognitionInfo& operator = (const RecognitionInfo& rhs);

 public:
  int32_t get_trackID() const {
    return this->trackID_;
  }

  void set_trackID(const int32_t& new_value) {
    this->trackID_ = new_value;
  }

  int32_t get_recognitionID() const {
    return this->recognitionID_;
  }

  void set_recognitionID(const int32_t& new_value) {
    this->recognitionID_ = new_value;
  }

  int32_t get_similarityScore() const {
    return this->similarityScore_;
  }

  void set_similarityScore(const int32_t& new_value) {
    this->similarityScore_ = new_value;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t trackID_;

  int32_t recognitionID_;

  int32_t similarityScore_;
};

#endif  // _RECOGNITION_INFO_H_
