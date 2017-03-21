// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "async_task_runner_instance.h"
#include <string>
#include "../process-singleton/process_singleton.h"

struct AsyncTaskRunnerTraits {
  static constexpr const char* shm_name = "node-realsense-shm-task_runner:";
};

typedef ProcessSingleton<AsyncTaskRunner,
    AsyncTaskRunnerTraits> runner_singleton_t;

static const int required_dummy = runner_singleton_t::SetupCleanupHooks();

class AsyncTaskRunnerInstanceD {
  friend class AsyncTaskRunnerInstance;

  static void DestroyInstance() {
    runner_singleton_t::Cleanup();
    delete runner_;
    runner_ = nullptr;
  }

  static AsyncTaskRunner* GetInstance() {
    if (!runner_) {
      if (!runner_singleton_t::QueryExistence()) {
        // Create one instance per one single OS process
        runner_singleton_t::SetInstance(new AsyncTaskRunner());
      }
      runner_ = runner_singleton_t::GetInstance();
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
