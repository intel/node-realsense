// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include <nan.h>
#include <node.h>

#include <memory>

class ContextRunner;

class Context {
 public:
  Context();

  ~Context();

  Context& operator = (const Context& rhs);

 public:
  v8::Handle<v8::Promise> getDeviceCount();

  v8::Handle<v8::Promise> getDevice(const int32_t& index);

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  std::unique_ptr<ContextRunner> context_runner_;
};

#endif  // _CONTEXT_H_
