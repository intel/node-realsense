// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_EXTRINSICS_H_
#define _COMMON_EXTRINSICS_H_

#include <node.h>
#include <v8.h>

#include <string>
#include <limits>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class Extrinsics {
 public:
  Extrinsics();

  Extrinsics(const float* rotations, const float* translations);

  Extrinsics(const Extrinsics& rhs);

  ~Extrinsics();

  Extrinsics& operator = (const Extrinsics& rhs);

  static const uint32_t ROTATION_SIZE = 3;
  static const uint32_t TRANSLATION_SIZE = 3;

 public:
  Float32ArrayHelper get_rotation() const {
    return this->rotation_;
  }

  void set_rotation(const Float32ArrayHelper& new_value) {
    DeepCopyFromTypedArrayHelper(new_value,
        rotation_store_, ROTATION_SIZE);
  }

  float GetRotation(size_t index) const {
    if (index < ROTATION_SIZE) {
      return rotation_store_[index];
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  void SetRotation(size_t index, float value) {
    if (index < ROTATION_SIZE) {
      rotation_store_[index] = value;
    }
  }

  void SetRotation(const float* src_ptr, size_t size = ROTATION_SIZE) {
    for (size_t index = 0; index < ROTATION_SIZE && index < size ; ++index) {
      rotation_store_[index] = src_ptr[index];
    }
  }

  Float32ArrayHelper get_translation() const {
    return this->translation_;
  }

  void set_translation(const Float32ArrayHelper& new_value) {
    DeepCopyFromTypedArrayHelper(new_value,
        translation_store_, TRANSLATION_SIZE);
  }

  float GetTranslation(size_t index) const {
    if (index < TRANSLATION_SIZE) {
      return translation_store_[index];
    }
    return std::numeric_limits<float>::quiet_NaN();
  }

  void SetTranslation(size_t index, float value) {
    if (index < TRANSLATION_SIZE) {
      translation_store_[index] = value;
    }
  }

  void SetTranslation(const float* src_ptr, size_t size = TRANSLATION_SIZE) {
    for (size_t index = 0; index < TRANSLATION_SIZE && index < size; ++index) {
      translation_store_[index] = src_ptr[index];
    }
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  void Setup();
  void CopyFrom(const Extrinsics& rhs);

 private:
  Float32ArrayHelper   rotation_;
  float                rotation_store_[ROTATION_SIZE];

  Float32ArrayHelper   translation_;
  float                translation_store_[TRANSLATION_SIZE];
};

#endif  // _COMMON_EXTRINSICS_H_
