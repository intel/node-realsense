// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "localization_info.h"

LocalizationInfo::LocalizationInfo() {
  label_ = "<no-label>";
  probability_ = 0;
}

LocalizationInfo::LocalizationInfo(const std::string& label, double probability,
    const Rect2D& roi, const Point3D& objectCenter) {
  label_ = label;
  probability_ = probability;
  roi_ = roi;
  objectCenter_ = objectCenter;
}

LocalizationInfo::LocalizationInfo(const LocalizationInfo& rhs) {
  CopyFrom(rhs);
}

LocalizationInfo::~LocalizationInfo() {
}

LocalizationInfo& LocalizationInfo::operator = (const LocalizationInfo& rhs) {
  CopyFrom(rhs);
  return *this;
}

void LocalizationInfo::CopyFrom(const LocalizationInfo& rhs) {
  if (&rhs != this) {
    label_ = rhs.label_;
    probability_ = rhs.probability_;
    roi_ = rhs.roi_;
    objectCenter_ = rhs.objectCenter_;
  }
}
