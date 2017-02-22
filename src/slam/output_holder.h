// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OUTPUT_HOLDER_H_
#define _OUTPUT_HOLDER_H_

#include <rs_sdk.h>

#include <memory>

#include "common/frame_data_assembler.h"
#include "event_info.h"
#include "tracking_result.h"
#include "utils.h"

class TrackedResultInternal {
 public:
  TrackedResultInternal() {}
  TrackedResultInternal(rs::slam::tracking_accuracy accuracy,
                        rs::slam::PoseMatrix4f pose)
      : accuracy_(accuracy), camera_pose_(pose) {}

  rs::slam::tracking_accuracy accuracy() {
    return accuracy_;
  }

  rs::slam::PoseMatrix4f pose() {
    return camera_pose_;
  }

  void set_accuracy(rs::slam::tracking_accuracy accuracy) {
    accuracy_ = accuracy;
  }

  void set_pose(rs::slam::PoseMatrix4f pose) {
    camera_pose_ = pose;
  }

  EventInfo* ToEventInfo() {
    ArrayHelper helper;
    helper.FromArrayT<float>(camera_pose_.m_data, 16);
    return new EventInfo(utils::AccuracyToString(accuracy_), helper);
  }

 private:
  rs::slam::tracking_accuracy accuracy_;
  rs::slam::PoseMatrix4f camera_pose_;
};

class OutputHolder{
 public:
  OutputHolder() {}

  ~OutputHolder() {
    frame_data_assembler_.reset();
  }

  void SetData(rs::slam::tracking_accuracy accuracy,
               rs::slam::PoseMatrix4f camera_pose,
               std::shared_ptr<FrameDataAssembler> frame_data_assembler) {
    accuracy_ = accuracy;
    camera_pose_ = camera_pose;
    frame_data_assembler_ = frame_data_assembler;
  }

  TrackingResult* ToTrackingResult() {
    auto result = new TrackingResult();
    if (!frame_data_assembler_) {
      return nullptr;
    }
    // Wrap output to trackingResult.
    result->set_accuracy(utils::AccuracyToString(accuracy_));
    ArrayHelper helper;
    helper.FromArrayT<float>(camera_pose_.m_data, 16);
    result->set_camera_pose(helper);
    result->set_frame_data(frame_data_assembler_->ToFrameData());
    return result;
  }

 private:
  rs::slam::tracking_accuracy accuracy_;
  rs::slam::PoseMatrix4f camera_pose_;
  std::shared_ptr<FrameDataAssembler> frame_data_assembler_;
};

#endif  // _OUTPUT_HOLDER_H_
