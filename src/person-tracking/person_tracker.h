// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_TRACKER_H_
#define _PERSON_TRACKER_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "face_recognition.h"
#include "frame_data.h"
#include "gen/array_helper.h"
#include "gen/camera_options.h"
#include "gen/generator_helper.h"
#include "gen/person_tracker_options.h"
#include "lying_pose_recognition.h"
#include "person_tracking.h"
#include "worker/person_tracker_runner_proxy.h"

class PersonTracker {
 public:
  PersonTracker();

  explicit PersonTracker(const PersonTrackerOptions& options);

  explicit PersonTracker(
      const PersonTrackerOptions& personTrackerOptions,
      const CameraOptions& cameraOptions);

  PersonTracker(const PersonTracker& rhs);

  ~PersonTracker();

  PersonTracker& operator = (const PersonTracker& rhs);

 public:
  std::string get_state() const {
    return PersonTrackerRunnerProxy::GetInstance()->GetStateString();
  }

  FaceRecognition* get_faceRecognition() const {
    return &this->faceRecognition_;
  }

  LyingPoseRecognition* get_lyingPoseRecognition() const {
    return &this->lyingPoseRecognition_;
  }

  PersonTracking* get_personTracking() const {
    return &this->personTracking_;
  }

  v8::Local<v8::Promise> start();

  v8::Local<v8::Promise> stop();

  v8::Local<v8::Promise> pause();

  v8::Local<v8::Promise> resume();

  v8::Local<v8::Promise> reset();

  v8::Local<v8::Promise> setPersonTrackerOptions(
      const PersonTrackerOptions& personTrackerOptions);

  v8::Local<v8::Promise> getPersonTrackerOptions();

  v8::Local<v8::Promise> setCameraOptions(
      const CameraOptions& cameraOptions);

  v8::Local<v8::Promise> getCameraOptions();

  v8::Local<v8::Promise> getFrameData();

  v8::Local<v8::Promise> getPersonInfo(const int32_t& trackID);

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
    PersonTrackerRunnerProxy::GetInstance()->SetJavaScriptThis(obj);
  }

 private:
  std::string state_;

  mutable FaceRecognition faceRecognition_;

  mutable LyingPoseRecognition lyingPoseRecognition_;

  mutable PersonTracking personTracking_;
};

#endif  // _PERSON_TRACKER_H_
