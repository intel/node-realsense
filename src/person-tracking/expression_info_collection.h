// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _EXPRESSION_INFO_COLLECTION_H_
#define _EXPRESSION_INFO_COLLECTION_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "expression_info.h"
#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class ExpressionInfoCollection {
 public:
  ExpressionInfoCollection();

  ExpressionInfoCollection(const ExpressionInfoCollection& rhs);

  ~ExpressionInfoCollection();

  ExpressionInfoCollection& operator = (
    const ExpressionInfoCollection& rhs);

 public:
  ArrayHelper get_infoCollection() const {
    return this->infoCollection_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable ArrayHelperStorage infoCollection_;
  friend class PersonDataInternal;
};

#endif  // _EXPRESSION_INFO_COLLECTION_H_
