// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <mutex> // NOLINT(*)

#include "async_task.h"
#include "gen/promise-helper.h"
#include "gen/widlnan_timer_helper.h"

class UniqueIDAllocator {
  UniqueIDAllocator() = delete;

 public:
  static std::string GetUniqueID() {
    std::lock_guard<std::mutex> lock(mutex_);
    const boost::uuids::uuid tag = boost::uuids::random_generator()();
    std::string uid = to_string(tag);
    return uid;
  }

 private:
  static std::mutex mutex_;
};
std::mutex UniqueIDAllocator::mutex_;

/////////////////////////////////////////////////////////////////////////////
AsyncTask::AsyncTask() {
  unique_id = UniqueIDAllocator::GetUniqueID();
  task_tag = "";
  task_state = Ready;
}

/////////////////////////////////////////////////////////////////////////////
class PromiseTaskD {
  friend class PromiseTask;
  PromiseHelper* promise_;
  PromiseMicrotaskRunner* runner_;
};

PromiseTask::PromiseTask() {
  d_ = new PromiseTaskD();
  d_->promise_ = new PromiseHelper();
  d_->runner_ = nullptr;
}

PromiseTask::~PromiseTask() {
  delete d_->promise_;
  delete d_->runner_;
  delete d_;
}

void PromiseTask::MainThreadDispatchResult() {
  Nan::HandleScope scope;

  if (TaskWasSuccessful()) {
    ResolvePromise();
  } else {
    RejectPromise();
  }

  // Workaround: when there is uv_async_t involved,
  //  we have to manually pump the microtasks
  if (d_->runner_) {
    d_->runner_->RunMicrotasks();
  }
  // End workaround
}

v8_promise_t PromiseTask::CreatePromise() {
  return d_->promise_->CreatePromise();
}

void PromiseTask::SetMicrotaskRunner(PromiseMicrotaskRunner* runner) {
  d_->runner_ = runner;
}

bool PromiseTask::TaskWasSuccessful() {
  return task_state == Successful;
}

v8_value_t PromiseTask::GetResolved() {
  return Nan::Undefined();
}

v8_value_t PromiseTask::GetRejected() {
  return Nan::Undefined();
}

void PromiseTask::ResolvePromise() {
  d_->promise_->ResolvePromise(GetResolved());
}

void PromiseTask::RejectPromise() {
  d_->promise_->RejectPromise(GetRejected());
}

/////////////////////////////////////////////////////////////////////////////
class FutureMakeCallback : public WIDLNANTimer {
 private:
  FutureMakeCallback(const std::string& event_name, v8_object_t js_this,
      v8_value_t data)
      : event_name_(event_name), js_this_(js_this), data_(data) {
  }

 public:
  virtual ~FutureMakeCallback() {
    js_this_.Reset();
    data_.Reset();
  }

  virtual bool TimerCallback(uint64_t countdown) {
    Nan::HandleScope scope;

    v8::Local<v8::Value> argv[2];
    argv[0] = Nan::New(event_name_.c_str()).ToLocalChecked();
    argv[1] = Nan::New(data_);
    Nan::MakeCallback(Nan::New(js_this_), "emit", 2, argv);

    return false;
  }

  static void CreateFutureMakeCallback(const std::string& event_name,
      v8_object_t js_this, v8_value_t data) {
    auto timer = new FutureMakeCallback(event_name, js_this, data);
    timer->SetTimeout(0);
  }

 private:
  std::string                   event_name_;
  Nan::Persistent<v8::Object>   js_this_;
  Nan::Persistent<v8::Value>    data_;
};

/////////////////////////////////////////////////////////////////////////////
class EventEmitterTaskD {
  friend class EventEmitterTask;
  Nan::Persistent<v8::Object>* js_this_;
  PromiseHelper* promise_;
  PromiseMicrotaskRunner* runner_;
};

EventEmitterTask::EventEmitterTask() {
  d_ = new EventEmitterTaskD();
  d_->js_this_ = nullptr;
  d_->promise_ = nullptr;
  d_->runner_ = nullptr;
}

EventEmitterTask::~EventEmitterTask() {
  if (d_->js_this_) {
    d_->js_this_->Reset();
  }
  delete d_->promise_;
  delete d_->js_this_;
  delete d_->runner_;
  delete d_;
}

v8_promise_t EventEmitterTask::CreatePromise() {
  if (!d_->promise_) {
    d_->promise_ = new PromiseHelper();
  }
  return d_->promise_->CreatePromise();
}

void EventEmitterTask::SetMicrotaskRunner(PromiseMicrotaskRunner* runner) {
  d_->runner_ = runner;
}

v8_value_t EventEmitterTask::GetResolved() {
  return Nan::Undefined();
}

v8_value_t EventEmitterTask::GetRejected() {
  return Nan::Undefined();
}

size_t EventEmitterTask::GetEventCount() {
  return 1;
}

bool EventEmitterTask::ShouldPopEvent(size_t event_index) {
  return true;
}

void EventEmitterTask::MainThreadDispatchResult() {
  if (d_->promise_) {
    Nan::HandleScope scope;
    if (task_state == Successful) {
      d_->promise_->ResolvePromise(GetResolved());
    } else {
      d_->promise_->RejectPromise(GetRejected());
    }
    delete d_->promise_;
    d_->promise_ = nullptr;

    // Workaround: when there is uv_async_t involved,
    //  we have to manually pump the microtasks
    if (d_->runner_) {
      d_->runner_->RunMicrotasks();
    }
    // End workaround
  }

  {
    Nan::HandleScope scope;
    size_t event_count = GetEventCount();
    for (size_t i=0; i < event_count; i++) {
      if (ShouldPopEvent(i)) {
        FutureMakeCallback::CreateFutureMakeCallback(GetEventName(i),
            Nan::New(*(d_->js_this_)),
            PopEventData(i));
      }
    }
  }
}

void EventEmitterTask::SetJavaScriptThis(v8_object_t object) {
  if (!d_->js_this_) {
    d_->js_this_ = new Nan::Persistent<v8::Object>();
  }
  d_->js_this_->Reset(object);
}


/////////////////////////////////////////////////////////////////////////////
DirectEventEmitterTask::DirectEventEmitterTask() {
  task_state = Successful;
}

DirectEventEmitterTask::~DirectEventEmitterTask() {
}

void DirectEventEmitterTask::MainThreadDispatchResult() {
  Nan::HandleScope scope;

  size_t event_count = GetEventCount();
  for (size_t i = 0; i < event_count; ++i) {
    if (ShouldPopEvent(i)) {
      FutureMakeCallback::CreateFutureMakeCallback(GetEventName(i),
          QueryJavaScriptThis(), PopEventData(i));
    }
  }
}

void DirectEventEmitterTask::WorkerThreadExecute() {
  throw std::runtime_error("WorkerThreadExecute() is not supposed"
    "to be called for class DirectEventEmitterTask{}.");
}

bool DirectEventEmitterTask::SkipWorkerThreadExecute() {
  return true;
}


/////////////////////////////////////////////////////////////////////////////
class BusyWaitingTaskD {
  BusyWaitingTaskD() {
    promise_ = new PromiseHelper();
    runner_ = nullptr;
    keep_waiting_ = true;
    is_fullfilled_ = false;
    cancel_ = false;
  }

  ~BusyWaitingTaskD() {
    delete promise_;
  }

  PromiseHelper* promise_;
  PromiseMicrotaskRunner* runner_;
  bool keep_waiting_;
  bool is_fullfilled_;
  bool cancel_;

  friend class BusyWaitingTask;
};

BusyWaitingTask::BusyWaitingTask() {
  d_ = new BusyWaitingTaskD();
}

BusyWaitingTask::~BusyWaitingTask() {
  delete d_->runner_;
  delete d_;
}

v8_promise_t BusyWaitingTask::CreatePromise() {
  return d_->promise_->CreatePromise();
}

void BusyWaitingTask::SetMicrotaskRunner(PromiseMicrotaskRunner* runner) {
  d_->runner_ = runner;
}

void BusyWaitingTask::MainThreadDispatchResult() {
  if (d_->keep_waiting_) {
    // Do nothing
  } else {
    Nan::HandleScope scope;
    if (d_->is_fullfilled_) {
      d_->promise_->ResolvePromise(GetResolved());
    } else {
      d_->promise_->RejectPromise(GetRejected());
    }

    // Workaround: when there is uv_async_t involved,
    //  we have to manually pump the microtasks
    if (d_->runner_) {
      d_->runner_->RunMicrotasks();
    }
    // End workaround
  }
}

void BusyWaitingTask::WorkerThreadExecute() {
  if (d_->cancel_) {
    task_state = Canceled;
  } else {
    d_->keep_waiting_ = KeepWaiting();
    if (!d_->keep_waiting_) {
      d_->is_fullfilled_ = IsFullfilled();
    }
    task_state = Successful;
  }
}

void BusyWaitingTask::ReloadIfNeeded() {
  if (d_->keep_waiting_) {
    task_state = Ready;
  }
  // Doing nothing means this task will be destroyed
}

void BusyWaitingTask::RequestCancel() {
  d_->cancel_ = true;
}

v8_value_t BusyWaitingTask::GetResolved() {
  return Nan::Undefined();
}

v8_value_t BusyWaitingTask::GetRejected() {
  return Nan::Undefined();
}

