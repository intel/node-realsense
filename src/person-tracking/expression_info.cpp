// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "expression_info.h"

ExpressionInfo::ExpressionInfo() {
  // TODO(widl-nan): init your members
}

ExpressionInfo::ExpressionInfo(const ExpressionInfo& rhs) {
  type_ = rhs.type_;
  confidence_ = rhs.confidence_;
}

ExpressionInfo::~ExpressionInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

ExpressionInfo& ExpressionInfo::operator = (const ExpressionInfo& rhs) {
  if (&rhs != this) {
    type_ = rhs.type_;
    confidence_ = rhs.confidence_;
  }

  return *this;
}

