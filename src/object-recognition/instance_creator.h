// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _INSTANCE_CREATOR_H_
#define _INSTANCE_CREATOR_H_

#include "gen/promise-helper.h"
#include "gen/camera_options.h"
#include "gen/object_recognition_options.h"

class InstanceCreator : public PromiseWorkQueueHelper {
 public:
  InstanceCreator(const DictionaryObjectRecognitionOptions& instance_options,
      const DictionaryCameraOptions& camera_options);

  ~InstanceCreator();

  virtual bool DoWork();
  virtual void OnWorkDone();

  virtual v8::Local<v8::Value> GetResolved();
  virtual v8::Local<v8::Value> GetRejected();

  static void DestroyInstance();

 private:
  class InstanceCreatorD* d_;
};

NAN_METHOD(CreateObjectRecognizerInstance);

#endif  // _INSTANCE_CREATOR_H_
