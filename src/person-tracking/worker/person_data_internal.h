// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_DATA_INTERNAL_H_
#define _WORKER_PERSON_DATA_INTERNAL_H_

#include <memory>
#include <vector>

#include "expression_info.h"
#include "gesture_info.h"
#include "person_info.h"
#include "skeleton_point_info.h"
#include "tracking_info.h"

class PersonDataInternal {
 public:
  PersonDataInternal();
  ~PersonDataInternal();

  PersonInfo* ToPersonInfo();
 private:
  std::vector<SkeletonPointInfo*> skeleton_joints_;
  std::shared_ptr<TrackingInfo> track_data_;
  std::shared_ptr<GestureInfo> ges_data_;

  // person face related data
  std::vector<ExpressionInfo*> expression_info_;
  std::vector<FaceLandmark*> landmarks_;
  int32_t landmark_confidence_;
  PoseEulerAngles head_pose_;
  bool has_head_pose_;

  std::shared_ptr<PoseInfo> pose_data_;
  friend class PersonTrackerAdapter;
};

#endif  // _WORKER_PERSON_DATA_INTERNAL_H_
