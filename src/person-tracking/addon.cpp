// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gen/nan__bounding_box2d_info.h"
#include "gen/nan__expression_info.h"
#include "gen/nan__expression_info_collection.h"
#include "gen/nan__face_landmark.h"
#include "gen/nan__face_landmark_info.h"
#include "gen/nan__frame_data.h"
#include "gen/nan__gesture_info.h"
#include "gen/nan__image.h"
#include "gen/nan__motion_sample.h"
#include "gen/nan__lying_pose_info.h"
#include "gen/nan__lying_pose_recognition.h"
#include "gen/nan__mask_info.h"
#include "gen/nan__orientation_info.h"
#include "gen/nan__person_face_info.h"
#include "gen/nan__person_info.h"
#include "gen/nan__person_recognition.h"
#include "gen/nan__person_recognizer_data.h"
#include "gen/nan__person_recognizer_data_with_status.h"
#include "gen/nan__person_registration_data.h"
#include "gen/nan__person_tracker.h"
#include "gen/nan__person_tracking.h"
#include "gen/nan__person_tracking_result.h"
#include "gen/nan__point2d.h"
#include "gen/nan__point3d.h"
#include "gen/nan__point_combined_info.h"
#include "gen/nan__pointing_data2d.h"
#include "gen/nan__pointing_data3d.h"
#include "gen/nan__pointing_info.h"
#include "gen/nan__pose_euler_angles.h"
#include "gen/nan__pose_info.h"
#include "gen/nan__range.h"
#include "gen/nan__rect2d.h"
#include "gen/nan__size2d.h"
#include "gen/nan__skeleton_info.h"
#include "gen/nan__skeleton_point_info.h"
#include "gen/nan__tracking_info.h"
#include "worker/person_tracker_creator.h"

void initModule(v8::Local<v8::Object> exports) {
  NanBoundingBox2DInfo::Init(exports);
  NanExpressionInfo::Init(exports);
  NanExpressionInfoCollection::Init(exports);
  NanFaceLandmark::Init(exports);
  NanFaceLandmarkInfo::Init(exports);
  NanFrameData::Init(exports);
  NanGestureInfo::Init(exports);
  NanImage::Init(exports);
  NanLyingPoseInfo::Init(exports);
  NanLyingPoseRecognition::Init(exports);
  NanMaskInfo::Init(exports);
  NanMotionSample::Init(exports);
  NanOrientationInfo::Init(exports);
  NanPersonFaceInfo::Init(exports);
  NanPersonRecognition::Init(exports);
  NanPersonRecognizerData::Init(exports);
  NanPersonRecognizerDataWithStatus::Init(exports);
  NanPersonRegistrationData::Init(exports);
  NanPersonInfo::Init(exports);
  NanPersonTracker::Init(exports);
  NanPersonTracking::Init(exports);
  NanPersonTrackingResult::Init(exports);
  NanPoint2D::Init(exports);
  NanPoint3D::Init(exports);
  NanPointCombinedInfo::Init(exports);
  NanPointingData2D::Init(exports);
  NanPointingData3D::Init(exports);
  NanPointingInfo::Init(exports);
  NanPoseEulerAngles::Init(exports);
  NanPoseInfo::Init(exports);
  NanRange::Init(exports);
  NanRect2D::Init(exports);
  NanSize2D::Init(exports);
  NanSkeletonPointInfo::Init(exports);
  NanSkeletonInfo::Init(exports);
  NanTrackingInfo::Init(exports);

  Nan::Export(exports, "createPersonTracker", CreatePersonTracker);
}

NODE_MODULE(realsense_person_tracking, initModule);
