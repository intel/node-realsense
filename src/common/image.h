// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_IMAGE_H_
#define _COMMON_IMAGE_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class Image {
 public:
  Image();

  Image(const Image& rhs) = delete;

  ~Image();

  Image& operator = (const Image& rhs);

 public:
  void Assign(Image* img);

  int32_t get_width() const {
    return this->width_;
  }

  void set_width(int32_t value) {
    width_ = value;
  }

  int32_t get_height() const {
    return this->height_;
  }

  void set_height(int32_t value) {
    height_ = value;
  }

  std::string get_format() const {
    return this->format_;
  }

  void set_format(std::string value) {
    format_ = value;
  }

  uint32_t get_timeStamp() const {
    return this->timestamp_;
  }

  void set_timeStamp(uint32_t value) {
    timestamp_ = value;
  }

  int32_t get_frameNumber() const {
    return this->frame_number_;
  }

  void set_frameNumber(int32_t value) {
    frame_number_ = value;
  }

  // When this method is called, the ownership of the data is transffered to
  // the caller
  ArrayBuffer get_data() {
    // As array buffer will be used in Nan::NewBuffer, and the ownership is
    // transfered to the caller, so mark this to not free it in destructor.
    own_data_ = false;
    return this->data_;
  }

  void set_data(ArrayBuffer value) {
    data_ = value;
    own_data_ = true;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t width_;

  int32_t height_;

  std::string format_;

  uint32_t timestamp_;

  int32_t frame_number_;

  ArrayBuffer data_;
  bool own_data_;
};

#endif  // _COMMON_IMAGE_H_
