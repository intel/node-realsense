// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_ASYNC_REQUEST_H_
#define _WORKER_ASYNC_REQUEST_H_

#include <memory>
#include <mutex>  //NOLINT

#include "utils.h"

class SlamRunner;

class AsyncRequest {
 public:
  AsyncRequest(std::shared_ptr<SlamRunner> runner, void* argument);
  explicit AsyncRequest(std::shared_ptr<SlamRunner> runner);
  ~AsyncRequest();
  void set_argument(void* argument);

  // Will be involked in worker thread.
  virtual void DoWork() = 0;

  // Will be involked in main thread.
  virtual void Resolve() = 0;

  v8::Local<v8::Promise> GetReturnPromise();

  uint32_t id() { return id_;}
  std::shared_ptr<SlamRunner> runner() {
    return runner_;
  }

 protected:
  std::mutex request_mutex_;
  void* argument_;
  void* result_data_;
  utils::Status result_status_;
  PromiseHelper promise_helper_;

  std::shared_ptr<SlamRunner> runner_;
  uint32_t id_;

  void ResolvePromise(v8::Local<v8::Value> value);
  void ResolvePromise();

 private:
  static uint32_t latest_id_;
  static uint32_t GetNewId();
};

#endif  // _WORKER_ASYNC_REQUEST_H_
