// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _FACE_RECOGNITION_H_
#define _FACE_RECOGNITION_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "worker/person_tracker_adapter.h"

class FaceRecognition {
 public:
  FaceRecognition();

  FaceRecognition(const FaceRecognition& rhs);

  ~FaceRecognition();

  FaceRecognition& operator = (const FaceRecognition& rhs);

 public:
  v8::Handle<v8::Promise> getRegisteredIDs();

  v8::Handle<v8::Promise> clearDatabase();

  v8::Handle<v8::Promise> exportDatabase();

  v8::Handle<v8::Promise> importDatabase(const ArrayBuffer& buf);

  v8::Handle<v8::Promise> recognizeAll();

  v8::Handle<v8::Promise> registerPerson(const int32_t& trackID);

  v8::Handle<v8::Promise> unRegisterPerson(const int32_t& recognitionID);

  v8::Handle<v8::Promise> isPersonRegistered(const int32_t& recognitionID);

  v8::Handle<v8::Promise> reinforceRegistration(
    const int32_t& trackID,
    const int32_t& recognitionID);

  v8::Handle<v8::Promise> recognize(const int32_t& trackID);

  v8::Handle<v8::Promise> querySimilarityScoreByID(
    const int32_t& trackID, const int32_t& recognitionID);

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  PersonTrackerAdapter* tracker_;
};

#endif  // _FACE_RECOGNITION_H_
