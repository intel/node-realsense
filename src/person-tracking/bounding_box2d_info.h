// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _BOUNDING_BOX2D_INFO_H_
#define _BOUNDING_BOX2D_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "common/geometry/rect2d.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class BoundingBox2DInfo {
 public:
  BoundingBox2DInfo();

  BoundingBox2DInfo(const BoundingBox2DInfo& rhs);

  ~BoundingBox2DInfo();

  BoundingBox2DInfo& operator = (const BoundingBox2DInfo& rhs);

 public:
  Rect2D* get_rect() const {
    return &this->rect_;
  }

  int32_t get_confidence() const {
    return this->confidence_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable Rect2D rect_;

  int32_t confidence_;
  friend class PersonTrackerAdapter;
};

#endif  // _BOUNDING_BOX2D_INFO_H_
