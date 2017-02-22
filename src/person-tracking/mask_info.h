// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _MASK_INFO_H_
#define _MASK_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class MaskInfo {
 public:
  MaskInfo();

  MaskInfo(const MaskInfo& rhs);

  ~MaskInfo();

  MaskInfo& operator = (const MaskInfo& rhs);

 public:
  int32_t get_width() const {
    return this->width_;
  }

  int32_t get_height() const {
    return this->height_;
  }

  int32_t get_timestamp() const {
    return this->timestamp_;
  }

  ArrayBuffer get_maskData() {
    // When get_maskData is called, the owner ship is transferred to the
    // caller, so we empty the internal buffer to avolid mem leak.
    auto ret = this->maskData_;
    memset(&this->maskData_, 0, sizeof(ArrayBuffer));
    return ret;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t width_;

  int32_t height_;

  int32_t timestamp_;

  ArrayBuffer maskData_;
  friend class PersonTrackerAdapter;
  friend class PersonDataInternal;
};

#endif  // _MASK_INFO_H_
