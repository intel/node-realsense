// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_MOTION_SAMPLE_H_
#define _COMMON_MOTION_SAMPLE_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class MotionSample {
 public:
  MotionSample();

  MotionSample(const MotionSample& rhs);

  ~MotionSample();

  MotionSample& operator = (const MotionSample& rhs);

  static const uint32_t DATA_SIZE = 3;

 public:
  double get_timeStamp() const {
    return this->timestamp_;
  }

  void set_timestamp(double timestamp) {
    this->timestamp_ = timestamp;
  }

  uint32_t get_frameNumber() const {
    return this->frame_number_;
  }

  void set_frame_number(uint32_t frameNumber) {
    this->frame_number_ = frameNumber;
  }

  Float32ArrayHelper get_data() const {
    return this->data_;
  }

  void SetData(size_t index, float value) {
    if (index < DATA_SIZE) {
      data_store_[index] = value;
    }
  }

  void SetData(const float* src_ptr,
      size_t size = DATA_SIZE) {
    for (size_t index = 0;
        index < DATA_SIZE && index < size;
        ++index) {
      data_store_[index] = src_ptr[index];
    }
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  void Setup();
  void CopyFrom(const MotionSample& rhs);

 private:
  double timestamp_;

  uint32_t frame_number_;

  Float32ArrayHelper data_;
  float data_store_[DATA_SIZE];
};

#endif  // _COMMON_MOTION_SAMPLE_H_
