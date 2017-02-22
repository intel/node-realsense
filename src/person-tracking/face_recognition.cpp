// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "face_recognition.h"

#include <memory>

FaceRecognition::FaceRecognition() {
  tracker_ = PersonTrackerAdapter::Instance();
}

FaceRecognition::FaceRecognition(const FaceRecognition& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in face_recognition.h
}

FaceRecognition::~FaceRecognition() {
  // TODO(widl-nan): do cleanup if necessary
}

FaceRecognition& FaceRecognition::operator = (const FaceRecognition& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }

  return *this;
}

v8::Handle<v8::Promise> FaceRecognition::getRegisteredIDs() {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::clearDatabase() {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::exportDatabase() {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::importDatabase(
  const ArrayBuffer& buf) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::recognizeAll() {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::registerPerson(
  const int32_t& trackID) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::unRegisterPerson(
  const int32_t& recognitionID) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::isPersonRegistered(
  const int32_t& recognitionID) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::reinforceRegistration(
  const int32_t& trackID,
  const int32_t& recognitionID) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::recognize(
  const int32_t& trackID) {
  // TODO(Shaoting): fill your code here
}

v8::Handle<v8::Promise> FaceRecognition::querySimilarityScoreByID(
  const int32_t& trackID,
  const int32_t& recognitionID) {
  // TODO(Shaoting): fill your code here
}
