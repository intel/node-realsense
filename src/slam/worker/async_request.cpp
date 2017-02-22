// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "async_request.h"
#include "slam_runner.h"

uint32_t AsyncRequest::latest_id_ = 0;
uint32_t AsyncRequest::GetNewId() {
  return latest_id_ = (latest_id_ < UINT32_MAX) ? (latest_id_ + 1) : 1;
}

AsyncRequest::AsyncRequest(std::shared_ptr<SlamRunner> runner, void* argument) {
  id_ = GetNewId();
  argument_ = argument;
  result_data_ = nullptr;
  runner_ = runner;
}

AsyncRequest::AsyncRequest(std::shared_ptr<SlamRunner> runner) {
  id_ = GetNewId();
  argument_ = nullptr;
  result_data_ = nullptr;
  runner_ = runner;
}

AsyncRequest::~AsyncRequest() {
  runner_.reset();
}

void AsyncRequest::set_argument(void* argument) {
  std::lock_guard<std::mutex> lock(request_mutex_);
  if (argument_ != nullptr) {
    delete argument_;
  }
  argument_ = argument;
}

// Will be involked in main thread.
void AsyncRequest::ResolvePromise(v8::Local<v8::Value> value) {
  if (result_status_.id() >= rs::core::status_no_error) {
    promise_helper_.ResolvePromise(value);
  } else {
    promise_helper_.RejectPromise(result_status_.message());
  }
  v8::Isolate::GetCurrent()->RunMicrotasks();
}

void AsyncRequest::ResolvePromise() {
  ResolvePromise(Nan::Undefined());
}

v8::Local<v8::Promise> AsyncRequest::GetReturnPromise() {
  return promise_helper_.CreatePromise();
}
