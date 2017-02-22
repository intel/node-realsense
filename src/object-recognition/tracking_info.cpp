// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "tracking_info.h"

TrackingInfo::TrackingInfo() {
}

TrackingInfo::TrackingInfo(const Rect2D& roi, const Point3D& object_center) {
  roi_ = roi;
  object_center_ = object_center;
}

TrackingInfo::TrackingInfo(const TrackingInfo& rhs) {
  CopyFrom(rhs);
}

TrackingInfo::~TrackingInfo() {
}

TrackingInfo& TrackingInfo::operator = (const TrackingInfo& rhs) {
  CopyFrom(rhs);
  return *this;
}

void TrackingInfo::CopyFrom(const TrackingInfo& rhs) {
  if (this != &rhs) {
    roi_ = rhs.roi_;
    object_center_ = rhs.object_center_;
  }
}
