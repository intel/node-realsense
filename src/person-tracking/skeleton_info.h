// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _SKELETON_INFO_H_
#define _SKELETON_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"
#include "skeleton_point_info.h"

class SkeletonInfo {
 public:
  SkeletonInfo();

  SkeletonInfo(const SkeletonInfo& rhs);

  ~SkeletonInfo();

  SkeletonInfo& operator = (const SkeletonInfo& rhs);

 public:
  ArrayHelper get_skeletonJoints() const {
    return this->skeletonJoints_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable ArrayHelperStorage skeletonJoints_;
  friend class PersonDataInternal;
};

#endif  // _SKELETON_INFO_H_
