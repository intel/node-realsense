// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class FrameData {
 public:
  FrameData();

  FrameData(const FrameData& rhs);

  ~FrameData();

  FrameData& operator = (const FrameData& rhs);

 public:
  std::string get_stream() const {
    return this->stream_;
  }

  ArrayBuffer get_data() const {
    return this->data_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::string stream_;

  ArrayBuffer data_;

  friend class DeviceRunner;
};

#endif  // _FRAME_DATA_H_
