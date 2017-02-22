// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _ASYNC_TASK_RUNNER_H_
#define _ASYNC_TASK_RUNNER_H_

#include <node.h>
#include <nan.h>

#include <string>

#include "async_task.h"
/*

How To Extend New Feature Using These threads/tasks/etc.


1. To implement a new method, e.g. instance.setOptions(),
    you need to define your own class, say: class SetOptionsTask{},
    inheriting from base class PromiseTask{},
    and then overriding the following functions:

     virtual void WorkerThreadExecute() override;

    The above is the function that will be called in worker thread.

2. If you need to carry some data/info to your worker thread task execution,
    then you need to define your own class, say class OptionsTaskPayload{},
    inheriting from class AsyncTaskPayload{}

   In your implementation of class SetOptionsTask, you can call GetPayload()
    to get the data/info you provided before

3. Call AsyncTaskRunner::PostPromiseTask() to execute your task

    PS. call AsyncTaskRunner::PostEventEmitterTask() if it's an event-based one

4. If you want to cancel a recurring task,
    you need to call AsyncTaskRunner::RequestCancelTask()

*/

/////////////////////////////////////////////////////////////////////////////
class AsyncTaskRunnerD;
class AsyncTaskRunner {
  AsyncTaskRunner(const AsyncTaskRunner&) = delete;
  AsyncTaskRunner operator = (const AsyncTaskRunner&) = delete;

 public:
  AsyncTaskRunner();
  ~AsyncTaskRunner();

  void PostTask(AsyncTask* task,
    AsyncTaskPayload* payload, const char* msgTxt);

  v8_promise_t PostPromiseTask(PromiseTask* task,
    AsyncTaskPayload* payload, const char* msgTxt);

  v8_promise_t PostEventEmitterTask(EventEmitterTask* task,
    AsyncTaskPayload* payload, v8_object_t object, const char* msgTxt);

  v8_promise_t PostBusyWaitingTask(BusyWaitingTask* task,
    AsyncTaskPayload* payload, const char* msgTxt);

  v8_promise_t RequestPauseTask(const std::string& task_id);

  v8_promise_t RequestResumeTask(const std::string& task_id);

  v8_promise_t RequestCancelTask(const std::string& task_id);

 private:
  AsyncTaskRunnerD * d_;
};


#endif  // _ASYNC_TASK_RUNNER_H_
