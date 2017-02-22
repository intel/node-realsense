// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _ASYNC_TASK_H_
#define _ASYNC_TASK_H_

#include <node.h>
#include <nan.h>

#include <memory>
#include <string>

#include "../type/v8_alias.h"

/////////////////////////////////////////////////////////////////////////////
// Base class for your payload info/data of your task
class AsyncTaskPayload {
 public:
  AsyncTaskPayload() {
  }

  virtual ~AsyncTaskPayload() {
  }

 protected:
  class AsyncTask* attached_to_;
  friend class AsyncTask;
};

/////////////////////////////////////////////////////////////////////////////
class AsyncTask {
 protected:
  AsyncTask();

 public:
  enum AsyncTaskState {
    Pending        = 0x0000,   // Waiting for precondition(s)
    Ready          = 0x0001,   // Ready for worker thread to execute
    Successful     = 0x1000,   // Executed and everything went fine
    Failed         = 0x1001,   // Executed and failed
    Canceled       = 0x1002,   // Canceled due to RequestCancel() method
    NoSuchTask     = 0x1003    // Can't find the specified task (anymore)
  };

  virtual ~AsyncTask() {}

  // Do the real work of a specific task
  //
  // This will be always invoked in worker thread,
  // so only deal with pure C++ data
  virtual void WorkerThreadExecute() = 0;


  // Determine WorkerThreadExecute() is necessary
  //
  // This will be always invoked in main thread
  //
  // Overload this to skip WorkerThreadExecute()
  virtual bool SkipWorkerThreadExecute() { return false; }

  // Dispatch the result of the task execution
  //
  // This will be always invoked in main thread,
  // so it's OK to do v8::Promise
  virtual void MainThreadDispatchResult() = 0;

  // Reload this task (optional)
  //
  // You can set it to Pending|Ready state
  // and it will be put to task queue again
  //
  // If you don't do this, the task is a one-time shot
  //
  // This will be always invoked in main thread
  virtual void ReloadIfNeeded() {}

  //
  // Override this if you want to cancel a task
  //
  // This can be invoked in main thread or worker thread
  virtual void RequestCancel() {}

  //
  // Override this if you want to pause a task
  //
  // This can be invoked in main thread or worker thread
  virtual void RequestPause() {}

  //
  // Override this if you want to resume a task
  //
  // This can be invoked in main thread or worker thread
  virtual void RequestResume() {}

 public:
  virtual AsyncTaskPayload* GetPayload() { return &(*payload_); }

  inline bool IsReady() const { return task_state == Ready; }
  inline bool IsPending() const { return task_state == Pending; }
  inline bool IsWorkerThreadExecuted() const { return task_state > Ready; }

  inline void SetPayload(AsyncTaskPayload* payload) {
    payload_.reset(payload);
    if (payload_.get()) {
      payload_->attached_to_ = this;
    }
    OnPayloadSet(payload);
  }

 public:
  std::string                         unique_id;
  std::string                         task_tag;
  AsyncTaskState                      task_state;

 protected:
  std::shared_ptr<AsyncTaskPayload>   payload_;

  virtual void OnPayloadSet(AsyncTaskPayload* payload) {}
};

/////////////////////////////////////////////////////////////////////////////
class PromiseMicrotaskRunner {
 public:
  PromiseMicrotaskRunner() {}
  virtual ~PromiseMicrotaskRunner() {}
  virtual void RunMicrotasks() = 0;
};

/////////////////////////////////////////////////////////////////////////////
class PromiseTaskBase {
 public:
  PromiseTaskBase() {}
  virtual ~PromiseTaskBase() {}

  virtual v8_promise_t CreatePromise() = 0;
  virtual void SetMicrotaskRunner(PromiseMicrotaskRunner* runner) = 0;
};

/////////////////////////////////////////////////////////////////////////////
class PromiseTaskD;
class PromiseTask : public AsyncTask, public PromiseTaskBase {
  PromiseTask(const PromiseTask&) = delete;
  PromiseTask& operator = (const PromiseTask&) = delete;

 public:
  PromiseTask();

  virtual ~PromiseTask();

  virtual void MainThreadDispatchResult();

  v8_promise_t CreatePromise() override;

  void SetMicrotaskRunner(PromiseMicrotaskRunner* runner) override;

  operator v8_promise_t () {
    return CreatePromise();
  }

 protected:
  // Override this if you want to make decision by yourself
  //
  // This will always be called in main thread
  virtual bool TaskWasSuccessful();

  // Override this function if you want to provide a value
  //  when resolving the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetResolved();

  // Override this function if you want to provide a value
  //  when rejecting the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetRejected();

 private:
  void ResolvePromise();
  void RejectPromise();

 private:
  class PromiseTaskD* d_;
};

/////////////////////////////////////////////////////////////////////////////
class JavaScriptThisQueryInterface {
 public:
  virtual ~JavaScriptThisQueryInterface() {}

 protected:
  // If JavaScript 'this' object is not set by SetJavaScriptThis() method,
  //  QueryJavaScriptThis() will be invoked to allow subclass
  //  to provide a JavaScript 'this' object
  //
  // This will always be called in main thread
  virtual v8_object_t QueryJavaScriptThis() = 0;
};

/////////////////////////////////////////////////////////////////////////////
class EventQueryInterface {
 public:
  virtual ~EventQueryInterface() {}

 protected:
  // Return the count of events you want to send to JavaScript
  //
  // ShouldPopEvent(), PopEventData() and GetEventName() will be called
  // with the index of each event.
  //
  // This will always be called in main thread
  //
  // TODO(Kenny): make this method with const modifier
  virtual size_t GetEventCount() = 0;

  // Return `true` if you want to send the event to JavaScript
  //
  // The 2 functions, PopEventData() & GetEventName(),
  //  won't be called for the event if this returns `false`
  //
  // This will always be called in main thread
  //
  // TODO(Kenny): make this method with const modifier
  virtual bool ShouldPopEvent(size_t event_index) = 0;

  // Assemble your event data and return it
  //
  // This will always be called in main thread
  virtual v8_value_t PopEventData(size_t event_index) = 0;

  // This will always be called in main thread
  virtual std::string GetEventName(size_t event_index) const = 0;
};


/////////////////////////////////////////////////////////////////////////////
class EventEmitterTaskD;
class EventEmitterTask
    : public AsyncTask, public EventQueryInterface, public PromiseTaskBase {
  EventEmitterTask(const EventEmitterTask&) = delete;
  EventEmitterTask& operator = (const EventEmitterTask&) = delete;

 public:
  EventEmitterTask();
  virtual ~EventEmitterTask();

  // This is supposed to be created once
  // Resolving this promise indicates the success of running the task
  // The result of the task is supposed to be sent via events
  v8_promise_t CreatePromise() override;

  void SetMicrotaskRunner(PromiseMicrotaskRunner* runner) override;

  operator v8_promise_t () {
    return CreatePromise();
  }

  void SetJavaScriptThis(v8_object_t object);

  virtual void MainThreadDispatchResult();

 protected:
  // Override this function if you want to provide a value
  //  when resolving the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetResolved();

  // Override this function if you want to provide a value
  //  when rejecting the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetRejected();

  // This will be called in EventEmitterTask::MainThreadDispatchResult()
  //
  // Return the count of events you want to send to JavaScript
  //
  // ShouldPopEvent(), PopEventData() and GetEventName() will be called
  // with the index of each event.
  //
  // This will always be called in main thread
  virtual size_t GetEventCount();

  // This will be called in EventEmitterTask::MainThreadDispatchResult()
  //
  // Return `true` if you want to send the event to JavaScript
  //
  // The 2 functions, PopEventData() & GetEventName(),
  //  won't be called for the event if this returns `false`
  //
  // This will always be called in main thread
  virtual bool ShouldPopEvent(size_t event_index);

  // This will be called in EventEmitterTask::MainThreadDispatchResult()
  //
  // Assemble your event data and return it
  //
  // This will always be called in main thread
  virtual v8_value_t PopEventData(size_t event_index) = 0;

  // This will be called in EventEmitterTask::MainThreadDispatchResult()
  //
  // This will always be called in main thread
  virtual std::string GetEventName(size_t event_index) const = 0;

 private:
  class EventEmitterTaskD* d_;
};


/////////////////////////////////////////////////////////////////////////////
class DirectEventEmitterTask
  : public AsyncTask,
    public JavaScriptThisQueryInterface,
    public EventQueryInterface {
 public:
  DirectEventEmitterTask();
  virtual ~DirectEventEmitterTask();

  virtual void MainThreadDispatchResult();
  virtual void WorkerThreadExecute();
  virtual bool SkipWorkerThreadExecute();
};


/////////////////////////////////////////////////////////////////////////////
class BusyWaitingTaskD;
class BusyWaitingTask : public AsyncTask, public PromiseTaskBase {
  BusyWaitingTask(const BusyWaitingTask&) = delete;
  BusyWaitingTask& operator = (const BusyWaitingTask&) = delete;

 public:
  BusyWaitingTask();
  virtual ~BusyWaitingTask();

  v8_promise_t CreatePromise() override;
  void SetMicrotaskRunner(PromiseMicrotaskRunner* runner) override;

  virtual void MainThreadDispatchResult();
  virtual void WorkerThreadExecute();

  virtual void ReloadIfNeeded();
  virtual void RequestCancel();

 protected:
  // Whether still waiting for the external condition
  // This will be always executed in worker thread
  virtual bool KeepWaiting() = 0;

  // Whether the external condition is fullfilled
  // This will be always executed in worker thread
  virtual bool IsFullfilled() = 0;

  // Override this function if you want to provide a value
  //  when resolving the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetResolved();

  // Override this function if you want to provide a value
  //  when rejecting the promise in this task
  //
  // This will always be called in main thread
  virtual v8_value_t GetRejected();

 private:
  class BusyWaitingTaskD* d_;
};

#endif  // _ASYNC_TASK_H_
