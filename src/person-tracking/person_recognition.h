// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_RECOGNITION_H_
#define _PERSON_RECOGNITION_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "worker/person_tracker_runner_proxy.h"

class PersonRecognition {
 public:
  PersonRecognition();

  PersonRecognition(const PersonRecognition& rhs);

  ~PersonRecognition();

  PersonRecognition& operator = (const PersonRecognition& rhs);

 public:
  v8::Handle<v8::Promise> clearDatabase();

  v8::Handle<v8::Promise> exportDatabase();

  v8::Handle<v8::Promise> importDatabase(const ArrayBuffer& buf);

  v8::Handle<v8::Promise> registerPerson(const int32_t& trackID);

  v8::Handle<v8::Promise> unRegisterPerson(const int32_t& recognitionID);

  v8::Handle<v8::Promise> recognitionIDExist(const int32_t& recognitionID);

  v8::Handle<v8::Promise> getAllRecognitionIDs();

  v8::Handle<v8::Promise> removePersonDescriptor(
      const int32_t& recognitionID, const int32_t& descriptorID);

  v8::Handle<v8::Promise> getPersonDescriptorIDs(const int32_t& recognitionID);

  v8::Handle<v8::Promise> reinforceRegistration(
      const int32_t& trackID, const int32_t& recognitionID);

  v8::Handle<v8::Promise> recognizePerson(const int32_t& trackID);

  v8::Handle<v8::Promise> querySimilarityScoreFromPerson(
      const int32_t& trackID, const int32_t& recognitionID);

  v8::Handle<v8::Promise> recognizeAllPersons();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
};

#endif  // _PERSON_RECOGNITION_H_
