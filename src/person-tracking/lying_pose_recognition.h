// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _LYING_POSE_RECOGNITION_H_
#define _LYING_POSE_RECOGNITION_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class LyingPoseRecognition {
 public:
  LyingPoseRecognition();

  LyingPoseRecognition(const LyingPoseRecognition& rhs);

  ~LyingPoseRecognition();

  LyingPoseRecognition& operator = (const LyingPoseRecognition& rhs);

 public:
  v8::Handle<v8::Promise> setRecognitionState(const std::string& state);

  v8::Handle<v8::Promise> getRecognitionState();

  v8::Handle<v8::Promise> getCandidatesCount();

  v8::Handle<v8::Promise> getCandidatesData();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
};

#endif  // _LYING_POSE_RECOGNITION_H_
