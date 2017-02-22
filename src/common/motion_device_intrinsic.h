// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_MOTION_DEVICE_INTRINSIC_H_
#define _COMMON_MOTION_DEVICE_INTRINSIC_H_

#include <node.h>
#include <v8.h>

#include <string>
#include <limits>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class MotionDeviceIntrinsic {
 public:
  MotionDeviceIntrinsic();

  MotionDeviceIntrinsic(const float* data,
      const float* noise_variances, const float* bias_variances);

  MotionDeviceIntrinsic(const MotionDeviceIntrinsic& rhs);

  ~MotionDeviceIntrinsic();

  MotionDeviceIntrinsic& operator = (const MotionDeviceIntrinsic& rhs);

  static const uint32_t DATA_SIZE = 12;
  static const uint32_t NOISE_VARIANCE_SIZE = 3;
  static const uint32_t BIAS_VARIANCE_SIZE = 3;

 public:
  Float32ArrayHelper get_data() const {
    return this->data_;
  }

  void set_data(const Float32ArrayHelper& new_value) {
    DeepCopyFromTypedArrayHelper(new_value,
        data_store_, DATA_SIZE);
  }

  float GetData(size_t index) const {
    if (index < DATA_SIZE) {
      return data_store_[index];
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  void SetData(size_t index, float value) {
    if (index < DATA_SIZE) {
      data_store_[index] = value;
    }
  }

  void SetData(const float* src_ptr, size_t size = DATA_SIZE) {
    for (size_t index = 0; index < DATA_SIZE && index < size ; ++index) {
      data_store_[index] = src_ptr[index];
    }
  }

  Float32ArrayHelper get_noiseVariances() const {
    return this->noiseVariances_;
  }

  void set_noiseVariances(const Float32ArrayHelper& new_value) {
    DeepCopyFromTypedArrayHelper(new_value,
        noise_variances_store_, NOISE_VARIANCE_SIZE);
  }

  float GetNoiseVariances(size_t index) const {
    if (index < NOISE_VARIANCE_SIZE) {
      return noise_variances_store_[index];
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  void SetNoiseVariances(size_t index, float value) {
    if (index < NOISE_VARIANCE_SIZE) {
      noise_variances_store_[index] = value;
    }
  }

  void SetNoiseVariances(const float* src_ptr,
      size_t size = NOISE_VARIANCE_SIZE) {
    for (size_t index = 0;
        index < NOISE_VARIANCE_SIZE && index < size;
        ++index) {
      noise_variances_store_[index] = src_ptr[index];
    }
  }

  Float32ArrayHelper get_biasVariances() const {
    return this->biasVariances_;
  }

  void set_biasVariances(const Float32ArrayHelper& new_value) {
    DeepCopyFromTypedArrayHelper(new_value,
        bias_variances_store_, BIAS_VARIANCE_SIZE);
  }

  float GetBiasVariances(size_t index) const {
    if (index < BIAS_VARIANCE_SIZE) {
      return bias_variances_store_[index];
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  void SetBiasVariances(size_t index, float value) {
    if (index < BIAS_VARIANCE_SIZE) {
      bias_variances_store_[index] = value;
    }
  }

  void SetBiasVariances(const float* src_ptr,
      size_t size = BIAS_VARIANCE_SIZE) {
    for (size_t index = 0;
        index < BIAS_VARIANCE_SIZE && index < size;
        ++index) {
      bias_variances_store_[index] = src_ptr[index];
    }
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  void Setup();
  void CopyFrom(const MotionDeviceIntrinsic& rhs);

 private:
  Float32ArrayHelper data_;            // Dimension: 16
  mutable float      data_store_[DATA_SIZE];

  Float32ArrayHelper noiseVariances_;  // Dimension: 3
  mutable float      noise_variances_store_[NOISE_VARIANCE_SIZE];

  Float32ArrayHelper biasVariances_;   // Dimension: 3
  mutable float      bias_variances_store_[BIAS_VARIANCE_SIZE];
};

#endif  // _COMMON_MOTION_DEVICE_INTRINSIC_H_
