// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/person_data_internal.h"

#include "gen/nan__expression_info.h"
#include "gen/nan__face_landmark.h"
#include "gen/nan__skeleton_point_info.h"
#include "worker/utils.h"

PersonDataInternal::PersonDataInternal()
    : landmark_confidence_(0),
      has_head_pose_(false)  {
}

PersonDataInternal::~PersonDataInternal() {
  for ( unsigned int i = 0; i < skeleton_joints_.size(); i++ ) {
    if (skeleton_joints_[i])
      delete skeleton_joints_[i];
  }

  for (unsigned int i = 0; i < expression_info_.size(); i++) {
    if (expression_info_[i])
      delete expression_info_[i];
  }

  for (unsigned int i=0; i < landmarks_.size(); i++) {
    if (landmarks_[i])
      delete landmarks_[i];
  }
}

PersonInfo* PersonDataInternal::ToPersonInfo() {
  PersonInfo* data = new PersonInfo();

  if (track_data_) {
    data->trackInfo_ = *track_data_;
    data->has_track_ = true;
  }

  if (ges_data_) {
    data->gestureInfo_ = *ges_data_;
    data->has_gesture_ = true;
  }

  if (skeleton_joints_.size()) {
    // make duplicated vector of skeleton points.
    std::vector<SkeletonPointInfo*> duplicate_points;
    for (auto iter = skeleton_joints_.begin(); iter != skeleton_joints_.end();
        ++iter) {
      duplicate_points.push_back(new SkeletonPointInfo(*(*iter)));
    }

    ArrayHelper helper;
    helper.FromArrayOfImplT<NanSkeletonPointInfo>(duplicate_points.begin(),
        duplicate_points.end());
    data->skeletonInfo_.skeletonJoints_.FromArrayHelper(helper);
    data->has_skeleton_ = true;
  }

  if (expression_info_.size()
      || landmarks_.size()
      || has_head_pose_) {
    data->has_face_ = true;
    if (expression_info_.size()) {
      // make duplicated vector of expressions
      std::vector<ExpressionInfo*> duplicate_expressions;
      for (auto iter = expression_info_.begin(); iter != expression_info_.end();
          ++iter) {
        duplicate_expressions.push_back(new ExpressionInfo(*(*iter)));
      }
      ArrayHelper helper;
      helper.FromArrayOfImplT<NanExpressionInfo>(duplicate_expressions.begin(),
          duplicate_expressions.end());
      data->faceInfo_.expressionInfo_.infoCollection_.FromArrayHelper(helper);
      data->faceInfo_.has_expression_ = true;
    }

    if (landmarks_.size()) {
      // make duplicated vector of landmarks.
      std::vector<FaceLandmark*> duplicate_landmarks;
      for (auto iter = landmarks_.begin(); iter != landmarks_.end(); ++iter) {
        duplicate_landmarks.push_back(new FaceLandmark(*(*iter)));
      }

      ArrayHelper helper;
      helper.FromArrayOfImplT<NanFaceLandmark>(duplicate_landmarks.begin(),
          duplicate_landmarks.end());
      data->faceInfo_.landmarkInfo_.landmarks_.FromArrayHelper(helper);
      data->faceInfo_.landmarkInfo_.confidence_ = landmark_confidence_;
      data->faceInfo_.has_landmark_ = true;
    }

    if (has_head_pose_) {
      data->faceInfo_.headPose_ = head_pose_;
      data->faceInfo_.has_head_pose_ = true;
    }
  }

  if (pose_data_) {
    data->poseInfo_ = *pose_data_;
    data->has_pose_ = true;
  }

  return data;
}
