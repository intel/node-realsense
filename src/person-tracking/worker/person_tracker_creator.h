// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_TRACKER_CREATOR_H_
#define _WORKER_PERSON_TRACKER_CREATOR_H_

#include <string>

#include "gen/camera_options.h"
#include "gen/person_tracker_options.h"
#include "gen/promise-helper.h"
#include "worker/person_tracker_runner_proxy.h"

class PersonTrackerCreator : public PromiseWorkQueueHelper {
 public:
  PersonTrackerCreator();
  ~PersonTrackerCreator() {}
  virtual bool DoWork();
  virtual void OnWorkDone();
  virtual v8::Local<v8::Value> GetResolved();
  virtual v8::Local<v8::Value> GetRejected();
  static void DestroyInstance();
  void SetTrackerOptions(
      const DictionaryPersonTrackerOptions& tracker_options);
  void SetCameraOptions(const DictionaryCameraOptions& camera_options);
 private:
  DictionaryPersonTrackerOptions tracker_options_;
  DictionaryCameraOptions camera_options_;
  bool have_tracker_options_;
  bool have_camera_options_;
  static Nan::Persistent<v8::Object>* tracker_object_;
  std::string fail_reason_;
};

NAN_METHOD(CreatePersonTracker);

#endif  // _WORKER_PERSON_TRACKER_CREATOR_H_
