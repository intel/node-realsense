// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _ASYNC_TASK_RUNNER_INSTANCE_
#define _ASYNC_TASK_RUNNER_INSTANCE_

#include "async_task_runner.h"

class AsyncTaskRunnerInstance {
  AsyncTaskRunnerInstance();
  ~AsyncTaskRunnerInstance();

 public:
  static AsyncTaskRunner* GetInstance();
  static void DestroyInstance();
};

#endif  // _ASYNC_TASK_RUNNER_INSTANCE_
