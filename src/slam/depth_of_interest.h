// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _DEPTH_OF_INTEREST_H_
#define _DEPTH_OF_INTEREST_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class DepthOfInterest {
 public:
  DepthOfInterest();

  DepthOfInterest(const DepthOfInterest& rhs);

  ~DepthOfInterest();

  DepthOfInterest& operator = (const DepthOfInterest& rhs);

 public:
  double get_minDepth() const {
    return this->minDepth_;
  }

  void set_minDepth(const double& new_value) {
    this->minDepth_ = new_value;
  }

  double get_maxDepth() const {
    return this->maxDepth_;
  }

  void set_maxDepth(const double& new_value) {
    this->maxDepth_ = new_value;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  double minDepth_;

  double maxDepth_;
};

#endif  // _DEPTH_OF_INTEREST_H_
