// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "tracking_info.h"

TrackingInfo::TrackingInfo()
    : id_(0),
      has_box_(false),
      has_center_(false),
      has_head_box_(false),
      has_seg_mask_(false),
      has_blob_mask_(false),
      has_orient_(false) {
  // TODO(widl-nan): init your members
}

TrackingInfo::TrackingInfo(const TrackingInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in tracking_info.h
}

TrackingInfo::~TrackingInfo() {
  // TODO(widl-nan): do cleanup if necessary
}

TrackingInfo& TrackingInfo::operator = (const TrackingInfo& rhs) {
  if (&rhs != this) {
    id_ = rhs.id_;
    if (rhs.has_box_) {
      has_box_ = true;
      boundingBox_ = rhs.boundingBox_;
    }

    if (rhs.has_center_) {
      has_center_ = true;
      center_ = rhs.center_;
    }

    if (rhs.has_head_box_) {
      has_head_box_ = true;
      headBoundingBox_ = rhs.headBoundingBox_;
    }

    if (rhs.has_seg_mask_) {
      has_seg_mask_ = true;
      segmentationMask_ = rhs.segmentationMask_;
    }

    if (rhs.has_blob_mask_) {
      has_blob_mask_ = true;
      blobMask_ = rhs.blobMask_;
    }

    if (rhs.has_orient_) {
      has_orient_ = true;
      personOrientation_ = rhs.personOrientation_;
    }
  }

  return *this;
}

