// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/camera_options.h"
#include "gen/object_recognition_options.h"
#include "gen/generator_helper.h"

class ObjectRecognizer {
 public:
  ObjectRecognizer();

  ObjectRecognizer(const ObjectRecognizer& rhs) = delete;

  ~ObjectRecognizer();

  ObjectRecognizer& operator = (const ObjectRecognizer& rhs);

 public:
  std::string get_state() const;

  v8::Local<v8::Promise> getCameraOptions();

  v8::Local<v8::Promise> getObjectRecognitionOptions();

  v8::Local<v8::Promise> setObjectRecognitionOptions(
      const ObjectRecognitionOptions& options);

  v8::Local<v8::Promise> setCameraOptions(const CameraOptions& options);

  v8::Local<v8::Promise> start();

  v8::Local<v8::Promise> pause();

  v8::Local<v8::Promise> resume();

  v8::Local<v8::Promise> stop();

  v8::Local<v8::Promise> reset();

  v8::Local<v8::Promise> restart();

  v8::Local<v8::Promise> getFrameData();

  v8::Local<v8::Promise> getRecognizableObjectNames();

  void SetJavaScriptThis(v8::Local<v8::Object> obj);

 private:
  class ObjectRecognizerD* d_;
};

#endif  // _INSTANCE_H_
