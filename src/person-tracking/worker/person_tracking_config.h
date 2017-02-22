// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_TRACKING_CONFIG_H_
#define _WORKER_PERSON_TRACKING_CONFIG_H_

#include <RealSense/PersonTracking/PersonTrackingConfiguration.h>

#include <memory>
#include <string>

#include "gen/camera_options.h"
#include "gen/person_tracker_options.h"

namespace PT = Intel::RealSense::PersonTracking;

class PersonTrackerOptionsHelper {
 public:
  PersonTrackerOptionsHelper() {}
  ~PersonTrackerOptionsHelper() {}

  void SetOptions(
      std::shared_ptr<DictionaryPersonTrackerOptions> options) {
    options_ = options;
  }

  bool IsTrackingEnabled() {
    if (options_) {
      if (options_->has_member_tracking) {
        const DictionaryTrackingOptions& tkoptions = options_->member_tracking;
        if (tkoptions.has_member_enable && tkoptions.member_enable
            || !tkoptions.has_member_enable)
          return true;
      }
    }
    return false;
  }

  bool IsTrackingDisabled() {
    if (options_) {
      if (options_->has_member_tracking) {
        const DictionaryTrackingOptions& tkoptions = options_->member_tracking;
        return tkoptions.has_member_enable && !tkoptions.member_enable;
      }
    }
    return false;
  }

  bool IsSkeletonEnabled() {
    if (options_) {
      if (options_->has_member_skeleton) {
        const DictionarySkeletonRecognitionOptions& skoptions =
            options_->member_skeleton;
        if (skoptions.has_member_enable && skoptions.member_enable
            || !skoptions.has_member_enable)
          return true;
      }
    }
    return false;
  }

  bool IsSkeletonDisabled() {
    if (options_) {
      if (options_->has_member_skeleton) {
        const DictionarySkeletonRecognitionOptions& skoptions =
            options_->member_skeleton;
        return skoptions.has_member_enable && !skoptions.member_enable;
      }
    }
    return false;
  }

  bool IsGestureEnabled() {
    if (options_) {
      if (options_->has_member_gesture) {
        const DictionaryGestureRecognitionOptions& gesoptions =
            options_->member_gesture;
        if (gesoptions.has_member_enable && gesoptions.member_enable
            || !gesoptions.has_member_enable)
          return true;
      }
    }
    return false;
  }

  bool IsGestureDisabled() {
    if (options_) {
      if (options_->has_member_gesture) {
        const DictionaryGestureRecognitionOptions& gesoptions =
            options_->member_gesture;
        return gesoptions.has_member_enable && !gesoptions.member_enable;
      }
    }
    return false;
  }

  bool IsHeadPoseEnabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_enableHeadPose
            && pfoptions.member_enableHeadPose)
          return true;
      }
    }
    return false;
  }

  bool IsHeadPoseDisabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_enableHeadPose
            && !pfoptions.member_enableHeadPose)
          return true;
      }
    }
    return false;
  }

  bool IsFaceLandmarkEnabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_enableFaceLandmarks
            && pfoptions.member_enableFaceLandmarks)
          return true;
      }
    }
    return false;
  }

  bool IsFaceLandmarkDisabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_enableFaceLandmarks
            && !pfoptions.member_enableFaceLandmarks)
          return true;
      }
    }
    return false;
  }

  bool IsExpressionEnabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_expression) {
          const DictionaryExpressionRecognitionOptions& expoptions =
              pfoptions.member_expression;
          if (expoptions.has_member_enable && expoptions.member_enable
            || !expoptions.has_member_enable) {
            return true;
          }
        }
      }
    }
    return false;
  }

  bool IsExpressionDisabled() {
    if (options_) {
      if (options_->has_member_personFace) {
        const DictionaryPersonFaceOptions& pfoptions =
            options_->member_personFace;
        if (pfoptions.has_member_expression) {
          const DictionaryExpressionRecognitionOptions& expoptions =
              pfoptions.member_expression;
          if (expoptions.has_member_enable && !expoptions.member_enable)
            return true;
        }
      }
    }
    return false;
  }

  bool IsRecognitionEnabled() {
    return false;
  }

  bool IsRecognitionDisabled() {
    return false;
  }

  bool IsPoseEnabled() {
    return false;
  }

  bool IsPoseDisabled() {
    return false;
  }

  bool IsLyingEnabled() {
    return false;
  }

  bool IsLyingDisabled() {
    return false;
  }

  std::shared_ptr<DictionaryPersonTrackerOptions> options_;
};

class PersonTrackingConfig {
 public:
  PersonTrackingConfig();
  ~PersonTrackingConfig() {}

  // This method create v8 objects, should be called in v8 main thread.
  bool GetConfigInfo(PersonTrackerOptions* options);
  void Dump();
  void Reset();
  enum Resolution {
    RESOLUTION_QVGA = 0,
    RESOLUTION_VGA, RESOLUTION_HD,
    RESOLUTION_FULLHD
  };

  Resolution color_resolution_;
  Resolution depth_resolution_;
  PersonTrackerOptionsHelper option_helper_;
  std::shared_ptr<DictionaryCameraOptions> camera_options_;

  static int GetResolutionWidth(Resolution res);
  static int GetResolutionHeight(Resolution res);
};

#endif  // _WORKER_PERSON_TRACKING_CONFIG_H_
