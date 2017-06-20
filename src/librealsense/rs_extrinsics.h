// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _RS_EXTRINSICS_H_
#define _RS_EXTRINSICS_H_

#include <librealsense/rs.hpp>
#include <node.h>
#include <v8.h>

#include <string>

#include "common/extrinsics.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class RSExtrinsics : public Extrinsics {
 public:
  RSExtrinsics() {}

  explicit RSExtrinsics(const RSExtrinsics& rhs) = delete;

  explicit RSExtrinsics(rs::extrinsics extrinsics);

  RSExtrinsics& operator = (const RSExtrinsics& rhs);

 public:
  bool get_isIdentity() const {
    return this->isIdentity_;
  }

  Float32ArrayHelper transform(const Float32ArrayHelper& point);  // NOLINT

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  rs::extrinsics extrinsics_;

  bool isIdentity_;

  Float32ArrayHelper transform_;
  float transform_store_[TRANSLATION_SIZE];
};

#endif  // _RS_EXTRINSICS_H_
