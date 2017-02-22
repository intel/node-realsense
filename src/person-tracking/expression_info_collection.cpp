// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "expression_info_collection.h"

ExpressionInfoCollection::ExpressionInfoCollection() {
  // TODO(widl-nan): init your members
}

ExpressionInfoCollection::ExpressionInfoCollection(
  const ExpressionInfoCollection& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in expression_info_collection.h
}

ExpressionInfoCollection::~ExpressionInfoCollection() {
  infoCollection_.FreeArray();
}

ExpressionInfoCollection& ExpressionInfoCollection::operator = (
  const ExpressionInfoCollection& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

