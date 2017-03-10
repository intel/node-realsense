// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "person_recognition.h"

PersonRecognition::PersonRecognition() {
  // TODO(widl-nan): init your members
}

PersonRecognition::PersonRecognition(const PersonRecognition& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in person_recognition.h
}

PersonRecognition::~PersonRecognition() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonRecognition& PersonRecognition::operator = (
    const PersonRecognition& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

v8::Handle<v8::Promise> PersonRecognition::clearDatabase() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> PersonRecognition::exportDatabase() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> PersonRecognition::importDatabase(
    const ArrayBuffer& buf) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> PersonRecognition::registerPerson(
    const int32_t& trackID) {
  return PersonTrackerRunnerProxy::GetInstance()->RegisterPerson(trackID);
}

v8::Handle<v8::Promise> PersonRecognition::unRegisterPerson(
    const int32_t& recognitionID) {
  return PersonTrackerRunnerProxy::GetInstance()->UnRegisterPerson(
      recognitionID);
}

v8::Handle<v8::Promise> PersonRecognition::recognitionIDExist(
    const int32_t& recognitionID) {
  return PersonTrackerRunnerProxy::GetInstance()->RecognitionIDExist(
      recognitionID);
}

v8::Handle<v8::Promise> PersonRecognition::getAllRecognitionIDs() {
  return PersonTrackerRunnerProxy::GetInstance()->GetAllRecognitionIDs();
}

v8::Handle<v8::Promise> PersonRecognition::removePersonDescriptor(
    const int32_t& recognitionID, const int32_t& descriptorID) {
  return PersonTrackerRunnerProxy::GetInstance()->RemovePersonDescriptor(
      recognitionID, descriptorID);
}

v8::Handle<v8::Promise> PersonRecognition::getPersonDescriptorIDs(
    const int32_t& recognitionID) {
  return PersonTrackerRunnerProxy::GetInstance()->GetPersonDescriptorIDs(
      recognitionID);
}

v8::Handle<v8::Promise> PersonRecognition::reinforceRegistration(
    const int32_t& trackID, const int32_t& recognitionID) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> PersonRecognition::recognizePerson(
    const int32_t& trackID) {
  return PersonTrackerRunnerProxy::GetInstance()->RecognizePerson(trackID);
}

v8::Handle<v8::Promise> PersonRecognition::querySimilarityScoreFromPerson(
    const int32_t& trackID, const int32_t& recognitionID) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> PersonRecognition::recognizeAllPersons() {
  return PersonTrackerRunnerProxy::GetInstance()->RecognizeAllPersons();
}
