// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "lying_pose_recognition.h"

LyingPoseRecognition::LyingPoseRecognition() {
  // TODO(widl-nan): init your members
}

LyingPoseRecognition::LyingPoseRecognition(
  const LyingPoseRecognition& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in lying_pose_recognition.h
}

LyingPoseRecognition::~LyingPoseRecognition() {
  // TODO(widl-nan): do cleanup if necessary
}

LyingPoseRecognition& LyingPoseRecognition::operator = (
  const LyingPoseRecognition& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

v8::Handle<v8::Promise> LyingPoseRecognition::setRecognitionState(
  const std::string& state) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> LyingPoseRecognition::getRecognitionState() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> LyingPoseRecognition::getCandidatesCount() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> LyingPoseRecognition::getCandidatesData() {
  // TODO(widl-nan): fill your code here
}

