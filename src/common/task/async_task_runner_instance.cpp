// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "async_task_runner_instance.h"

class AsyncTaskRunnerInstanceD {
  friend class AsyncTaskRunnerInstance;

  static void DestroyInstance() {
    delete runner_;
    runner_ = nullptr;
  }

  static AsyncTaskRunner* GetInstance() {
    if (!runner_) {
      runner_ = new AsyncTaskRunner();
    }

    return runner_;
  }

  static AsyncTaskRunner* runner_;
};

AsyncTaskRunner* AsyncTaskRunnerInstanceD::runner_ = nullptr;

AsyncTaskRunnerInstance::AsyncTaskRunnerInstance() {
}

AsyncTaskRunnerInstance::~AsyncTaskRunnerInstance() {
}

AsyncTaskRunner* AsyncTaskRunnerInstance::GetInstance() {
  return AsyncTaskRunnerInstanceD::GetInstance();
}

void AsyncTaskRunnerInstance::DestroyInstance() {
  return AsyncTaskRunnerInstanceD::DestroyInstance();
}
