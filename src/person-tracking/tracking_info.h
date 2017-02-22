// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _TRACKING_INFO_H_
#define _TRACKING_INFO_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "bounding_box2d_info.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "orientation_info.h"
#include "point_combined_info.h"
#include "pose_euler_angles.h"
#include "mask_info.h"

class TrackingInfo {
 public:
  TrackingInfo();

  TrackingInfo(const TrackingInfo& rhs);

  ~TrackingInfo();

  TrackingInfo& operator = (const TrackingInfo& rhs);

 public:
  int32_t get_id() const {
    return this->id_;
  }

  BoundingBox2DInfo* get_boundingBox() const {
    return has_box_ ? &this->boundingBox_ : nullptr;
  }

  PointCombinedInfo* get_center() const {
    return has_center_ ? &this->center_ : nullptr;
  }

  BoundingBox2DInfo* get_headBoundingBox() const {
    return has_head_box_ ? &this->headBoundingBox_ : nullptr;
  }

  MaskInfo* get_segmentationMask() const {
    return has_seg_mask_ ? &this->segmentationMask_ : nullptr;
  }

  MaskInfo* get_blobMask() const {
    return has_blob_mask_ ? &this->blobMask_ : nullptr;
  }

  OrientationInfo* get_personOrientation() const {
    return has_orient_ ? &this->personOrientation_ : nullptr;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t id_;

  mutable BoundingBox2DInfo boundingBox_;

  mutable PointCombinedInfo center_;

  mutable BoundingBox2DInfo headBoundingBox_;

  mutable MaskInfo segmentationMask_;

  mutable MaskInfo blobMask_;

  mutable OrientationInfo personOrientation_;

  bool has_box_;
  bool has_center_;
  bool has_head_box_;
  bool has_seg_mask_;
  bool has_blob_mask_;
  bool has_orient_;
  friend class PersonTrackerAdapter;
  friend class PersonDataInternal;
};

#endif  // _TRACKING_INFO_H_
