// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "async_task_runner.h"

#include <memory>
#include <string>
#include <algorithm>
#include <vector>

#include "gen/widlnan_timer_helper.h"

/////////////////////////////////////////////////////////////////////////////
class RAIIMutex {
 public:
  explicit RAIIMutex(uv_mutex_t* mutex) {
    auto_unlock_ = true;
    mutex_ = mutex;
    Lock();
  }

  ~RAIIMutex() {
    if (auto_unlock_) {
      Unlock();
    }
  }

  void DismissAutoUnlock() {
    auto_unlock_ = false;
  }

 private:
  uv_mutex_t*   mutex_;
  bool          auto_unlock_;

  void Lock() { uv_mutex_lock(mutex_); }
  void Unlock() { uv_mutex_unlock(mutex_); }
};

/////////////////////////////////////////////////////////////////////////////
class AsyncNotifyBase {
 public:
  explicit AsyncNotifyBase(bool useDefaultLoop = false) {
    use_default_loop_ = useDefaultLoop;
    if (useDefaultLoop) {
      loop_ = uv_default_loop();
    } else {
      loop_ = reinterpret_cast<uv_loop_t*>(malloc(sizeof (uv_loop_t)));
      uv_loop_init(loop_);
    }
    async_ = reinterpret_cast<uv_async_t*>(malloc(sizeof (uv_async_t)));
    uv_async_init(loop_, async_, AsyncProc);
    async_->data = this;
    next_ = nullptr;
  }

  virtual ~AsyncNotifyBase() {
    if (use_default_loop_) {
      uv_close(reinterpret_cast<uv_handle_t*>(async_),
        [](uv_handle_t* handle) -> void {free(handle);});

    } else {
      uv_close(reinterpret_cast<uv_handle_t*>(async_),
        [](uv_handle_t* handle) -> void {free(handle);});

      uv_loop_close(loop_);
      free(loop_);
    }
  }

  static void AsyncProc(uv_async_t *handle) {
    if (AsyncNotifyBase* me = reinterpret_cast<AsyncNotifyBase*>
        (handle->data)) {
      if (me->data_) {
        std::string debugTip = me->OnAsyncNotification(me->data_);

        if (me->next_) {
          me->next_->SendAsyncNotification(me->data_);
        }
      }
    }
  }

  void SendAsyncNotification(const char* msg) {
    data_ = msg;
    uv_async_send(async_);  // This will trigger "AsyncProc()"
  }

  // Important Notice: there is no loop detection
  void SetNextInLine(AsyncNotifyBase* next) {
    next_ = next;
  }

  void SetName(const std::string& name) {
    name_ = name;
  }

  virtual std::string OnAsyncNotification(const char* msg) = 0;

 protected:
  std::string   name_;
  uv_loop_t*    loop_;
  uv_async_t*   async_;
  const char*   data_;
  bool          use_default_loop_;

  class AsyncNotifyBase*  next_;
};


/////////////////////////////////////////////////////////////////////////////
class AsyncTaskQueue {
 public:
  AsyncTaskQueue() {
    task_queue_mutex = reinterpret_cast<uv_mutex_t*>(
        malloc(sizeof(uv_mutex_t)));
    uv_mutex_init(task_queue_mutex);
  }

  ~AsyncTaskQueue() {
    uv_mutex_destroy(task_queue_mutex);
    free(task_queue_mutex);
  }

  bool AddTask(AsyncTask* task) {
    RAIIMutex guard(task_queue_mutex);
    task_queue.push_back(task);
    return true;
  }

  void RemoveTask(AsyncTask* task) {
    RAIIMutex guard(task_queue_mutex);
    auto pos = std::find(task_queue.begin(), task_queue.end(), task);
    task_queue.erase(pos);
  }

  void MoveTaskToTail(AsyncTask* task) {
    RAIIMutex guard(task_queue_mutex);
    auto pos = std::find(task_queue.begin(), task_queue.end(), task);
    if (pos != task_queue.end()) {
      std::rotate(pos, pos + 1, task_queue.end());
    }
  }

  template<class IterType>
  bool AddTask(IterType first, IterType last) {
    RAIIMutex guard(task_queue_mutex);
    std::for_each(first, last, [this](AsyncTask* task){
      this->task_queue.push_back(task);
    });
    return true;
  }

  bool IsTaskPending(const std::string& id) {
    auto pendings = GetPendingTasks();
    return std::find_if(pendings.begin(),
                        pendings.end(),
                        [id](AsyncTask* t) {
      return t->unique_id == id;
    }) != pendings.end();
  }

  bool IsTaskExisting(const std::string& id) {
    RAIIMutex guard(task_queue_mutex);
    return std::find_if(task_queue.begin(),
                        task_queue.end(),
                        [id](AsyncTask* t) {
      return t->unique_id == id;
    })
      != task_queue.end();
  }

  AsyncTask::AsyncTaskState GetTaskState(const std::string& id) {
    RAIIMutex guard(task_queue_mutex);
    auto it = std::find_if(task_queue.begin(),
                           task_queue.end(),
                           [id](AsyncTask* t) {
      return t->unique_id == id;
    });
    if (it != task_queue.end()) {
      return (*it)->task_state;
    }
    return AsyncTask::AsyncTaskState::NoSuchTask;
  }

  void RequestPauseTask(const std::string& id) {
    RAIIMutex guard(task_queue_mutex);
    std::for_each(task_queue.begin(),
                  task_queue.end(),
                  [id](AsyncTask* t) {
      if (t->unique_id == id) {
        t->RequestPause();
      }
    });
  }

  void RequestResumeTask(const std::string& id) {
    RAIIMutex guard(task_queue_mutex);
    std::for_each(task_queue.begin(),
                  task_queue.end(),
                  [id](AsyncTask* t) {
      if (t->unique_id == id) {
        t->RequestResume();
      }
    });
  }

  void RequestCancelTask(const std::string& id) {
    RAIIMutex guard(task_queue_mutex);
    std::for_each(task_queue.begin(),
                  task_queue.end(),
                  [id](AsyncTask* t) {
      if (t->unique_id == id) {
        t->RequestCancel();
      }
    });
  }

  // Get a copy of all AsyncTask pointers
  std::vector<AsyncTask*> GetAllTasks() {
    RAIIMutex guard(task_queue_mutex);
    return task_queue;
  }

  bool IsQueueEmpty() const {
    RAIIMutex guard(task_queue_mutex);
    return task_queue.empty();
  }

  // Fiter out those ones who are ready to "task->WorkerThreadExecute()"
  std::vector<AsyncTask*> GetReadyTasks() {
    RAIIMutex guard(task_queue_mutex);

    std::vector<AsyncTask*> tasks;
    std::copy_if(task_queue.begin(), task_queue.end(),
        std::back_inserter(tasks), [](AsyncTask* task) {
      return task->IsReady();
    });

    return tasks;
  }

  void ForceSetTaskState(const std::string& task_id,
      AsyncTask::AsyncTaskState state) {
    RAIIMutex guard(task_queue_mutex);
    std::for_each(task_queue.begin(), task_queue.end(),
        [task_id, state](AsyncTask* t) {
      if (t->unique_id == task_id) {
        t->task_state = state;
      }
    });
  }

  // Fiter out those ones that are pending
  std::vector<AsyncTask*> GetPendingTasks() {
    RAIIMutex guard(task_queue_mutex);

    std::vector<AsyncTask*> tasks;
    std::copy_if(task_queue.begin(), task_queue.end(),
        std::back_inserter(tasks), [](AsyncTask* task) {
      return task->IsPending();
    });

    return tasks;
  }

  // Get all task(s) that are ready to
  //  "task->MainThreadDispatchResult()" from the queue
  // Note: caller of this function is responsible to delete tasks
  std::vector<AsyncTask*> CopyExecutedTasks() {
    RAIIMutex guard(task_queue_mutex);

    std::vector<AsyncTask*> tasks;

    // Copy those already-executed tasks
    std::copy_if(task_queue.begin(), task_queue.end(),
        std::back_inserter(tasks), [](AsyncTask* task) {
      return task->IsWorkerThreadExecuted();
    });

    return tasks;
  }

 private:
  std::vector<AsyncTask*>  task_queue;
  uv_mutex_t*              task_queue_mutex;
};

/////////////////////////////////////////////////////////////////////////////
class AsyncTaskQueueConsumer {
 protected:
  explicit AsyncTaskQueueConsumer(std::shared_ptr<AsyncTaskQueue> queue) {
    queue_ = queue;
  }

  virtual ~AsyncTaskQueueConsumer() {}

 public:
  AsyncTaskQueue& GetTaskQueue() { return *queue_; }

 private:
  std::shared_ptr<AsyncTaskQueue> queue_;
};

/////////////////////////////////////////////////////////////////////////////
class AsyncTaskReloader {
 public:
  AsyncTaskReloader() {}
  virtual ~AsyncTaskReloader() {}

  virtual void ReloadTask() = 0;
};

/////////////////////////////////////////////////////////////////////////////
class AsyncQueueEmptyNotification {
 public:
  virtual ~AsyncQueueEmptyNotification() {}

  virtual void OnAsyncQueueEmpty() = 0;
};

/////////////////////////////////////////////////////////////////////////////
class MainThread : public AsyncNotifyBase, public AsyncTaskQueueConsumer {
 public:
  explicit MainThread(const std::string& name,
      std::shared_ptr<AsyncTaskQueue> queue)
    : AsyncNotifyBase(true), AsyncTaskQueueConsumer(queue) {
    SetName(name);
    task_reloader_ = nullptr;
    target_ = nullptr;
  }

  void SetQueueEmptyNotifyTarget(AsyncQueueEmptyNotification* target) {
    target_ = target;
  }

  virtual std::string OnAsyncNotification(const char* msg) {
    // Now it time to filter out the tasks and deal with the result
    std::vector<AsyncTask*> tasks = GetTaskQueue().CopyExecutedTasks();
    bool reload = false;
    std::for_each(tasks.begin(), tasks.end(), [this, &reload](AsyncTask* task) { // NOLINT(*)
      // task->task_state is either Successful OR Failed
      task->MainThreadDispatchResult();
      task->ReloadIfNeeded();
      if (task->task_state > AsyncTask::Ready) {
        this->GetTaskQueue().RemoveTask(task);
        delete task;
      } else {
        // A reloaded task is the last one in queue
        this->GetTaskQueue().MoveTaskToTail(task);
        reload = true;
      }
    });

    if (reload && task_reloader_) {
      task_reloader_->ReloadTask();
    }

    if (target_ && GetTaskQueue().IsQueueEmpty()) {
      target_->OnAsyncQueueEmpty();
    }

    return std::string("main thread done processing tasks.");
  }

  void SetTaskReloader(AsyncTaskReloader* reloader) {
    task_reloader_ = reloader;
  }

 private:
  AsyncTaskReloader* task_reloader_;
  AsyncQueueEmptyNotification* target_;
};

/////////////////////////////////////////////////////////////////////////////
class WorkerThread : public AsyncNotifyBase,
    public AsyncTaskQueueConsumer, public AsyncTaskReloader {
 public:
  explicit WorkerThread(
      const std::string& name, std::shared_ptr<AsyncTaskQueue> queue)
     : AsyncNotifyBase(false), AsyncTaskQueueConsumer(queue) {
    uv_thread_create(&thread_, ThreadProc, loop_);
    SetName(name);
  }

  virtual ~WorkerThread() {
    // Break the notification chain
    SetNextInLine(nullptr);

    // Make the thread stop
    StopThread();
  }

  static void ThreadProc(void* data) {
    uv_loop_t* loop = reinterpret_cast<uv_loop_t*>(data);
    uv_run(loop, UV_RUN_DEFAULT);
  }

  virtual std::string OnAsyncNotification(const char* msg) {
    auto tasks = GetTaskQueue().GetReadyTasks();
    std::for_each(tasks.begin(), tasks.end(), [](AsyncTask* task){
      // Only WorkerThreadExecute() tasks which are in ready state
      if (!task->SkipWorkerThreadExecute()) {
        task->WorkerThreadExecute();
      }
    });

    // String content doesn't matter, just debug tip
    return std::string("worker thread done processing tasks.");
  }

  void StopThread() {
    uv_stop(loop_);
    data_ = nullptr;
    try {
      // the thread can be already stopped after uv_stop()
      uv_async_send(async_);
      uv_thread_join(&thread_);
    } catch (...) {
      // Just absorb them all
    }
  }

  virtual void ReloadTask() {
    SendAsyncNotification("{{RELOADED TASK}}");
  }

 private:
  uv_thread_t thread_;
};

/////////////////////////////////////////////////////////////////////////////
class BusyWaitingPauseTask : public BusyWaitingTask {
 public:
  BusyWaitingPauseTask(AsyncTaskQueue* queue, const std::string& task) {
    task_queue_ = queue;
    requested_task_ = task;
  }

  virtual ~BusyWaitingPauseTask() {
    // Do not destroy task_queue_ OR requested_task_ here
  }

 protected:
  virtual bool KeepWaiting() {
    auto state = task_queue_->GetTaskState(requested_task_);
    if (state == AsyncTask::Pending) {
      return false;
    }
    return true;
  }

  virtual bool IsFullfilled() {
    return task_queue_->GetTaskState(requested_task_) == AsyncTask::Pending;
  }

 private:
  AsyncTaskQueue* task_queue_;
  std::string     requested_task_;
};

/////////////////////////////////////////////////////////////////////////////
class BusyWaitingResumeTask : public BusyWaitingTask {
 public:
  BusyWaitingResumeTask(AsyncTaskQueue* queue, const std::string& task) {
    task_queue_ = queue;
    requested_task_ = task;
  }

  virtual ~BusyWaitingResumeTask() {
    // Do not destroy task_queue_ OR requested_task_ here
  }

 protected:
  virtual bool KeepWaiting() {
    auto state = task_queue_->GetTaskState(requested_task_);
    if (state == AsyncTask::Pending) {
      return true;
    }
    return false;
  }

  virtual bool IsFullfilled() {
    return task_queue_->GetTaskState(requested_task_) != AsyncTask::Pending;
  }

 private:
  AsyncTaskQueue* task_queue_;
  std::string     requested_task_;
};

/////////////////////////////////////////////////////////////////////////////
class BusyWaitingCanceledTask : public BusyWaitingTask {
 public:
  BusyWaitingCanceledTask(AsyncTaskQueue* queue, const std::string& task) {
    task_queue_ = queue;
    requested_task_ = task;
  }

  virtual ~BusyWaitingCanceledTask() {
    // Do not destroy task_queue_ OR requested_task_ here
  }

 protected:
  virtual bool KeepWaiting() {
    return task_queue_->IsTaskExisting(requested_task_);
  }

  virtual bool IsFullfilled() {
    return !task_queue_->IsTaskExisting(requested_task_);
  }

 private:
  AsyncTaskQueue* task_queue_;
  std::string     requested_task_;
};

/////////////////////////////////////////////////////////////////////////////
class MicrotaskRunnerTimer : public WIDLNANTimer {
 public:
  MicrotaskRunnerTimer() {}
  virtual ~MicrotaskRunnerTimer() {}

  virtual bool TimerCallback(uint64_t countdown) {
    v8::Isolate::GetCurrent()->RunMicrotasks();
    return false;
  }
};

/////////////////////////////////////////////////////////////////////////////
class PromiseMicrotaskRunnerImpl : public PromiseMicrotaskRunner {
 public:
  PromiseMicrotaskRunnerImpl() {}
  virtual ~PromiseMicrotaskRunnerImpl() {}

  virtual void RunMicrotasks() {
    auto timer = new MicrotaskRunnerTimer();
    timer->SetTimeout(0);
  }
};

/////////////////////////////////////////////////////////////////////////////
class DestroyAsyncQueue {
 public:
  virtual ~DestroyAsyncQueue() {}

  virtual void DoDestroy() = 0;
};

/////////////////////////////////////////////////////////////////////////////
class DestroyThreadsTimer : public WIDLNANTimer {
 public:
  explicit DestroyThreadsTimer(DestroyAsyncQueue* executor) {
    executor_ = executor;
  }
  virtual ~DestroyThreadsTimer() {}

  virtual bool TimerCallback(uint64_t countdown) {
    executor_->DoDestroy();
    return false;
  }
 private:
  DestroyAsyncQueue* executor_;
};

/////////////////////////////////////////////////////////////////////////////
class AsyncTaskRunnerD :
    public AsyncQueueEmptyNotification,
    public DestroyAsyncQueue {
  friend class AsyncTaskRunner;
  friend class DestroyThreadsTimer;

  AsyncTaskRunnerD() {
    abort_destroy_threads_ = false;
    queue_ = std::make_shared<AsyncTaskQueue>();
    main_thread_ = nullptr;
    worker_thread_ = nullptr;
    TouchThreads();
  }

  ~AsyncTaskRunnerD() {
    DestroyThreads();
    queue_.reset();
  }

  virtual void OnAsyncQueueEmpty() {
    abort_destroy_threads_ = false;
    main_thread_->SetQueueEmptyNotifyTarget(nullptr);

    auto timer = new DestroyThreadsTimer(this);
    timer->SetTimeout(0);
  }

  virtual void DoDestroy() {
    CheckAndDestroyThreads();
  }

  void SendAsyncNotification(AsyncTask* task, const char* msg) {
    abort_destroy_threads_ = true;
    TouchThreads();

    if (task->SkipWorkerThreadExecute()) {
      // TODO(Kenny): check task state, should be neigher Ready/Pending
      main_thread_->SendAsyncNotification(msg);
    } else {
      // TODO(Kenny): check task state, should be either Ready/Pending
      worker_thread_->SendAsyncNotification(msg);
    }
  }

  void TouchThreads() {
    if (!main_thread_) {
      main_thread_ = new MainThread("main", queue_);
      main_thread_->SetTaskReloader(nullptr);
      main_thread_->SetQueueEmptyNotifyTarget(this);
    }

    if (!worker_thread_) {
      worker_thread_ = new WorkerThread("worker", queue_);
      worker_thread_->SetNextInLine(nullptr);
      worker_thread_->SetNextInLine(main_thread_);

      main_thread_->SetTaskReloader(worker_thread_);
    }
  }

  void DestroyThreads() {
    if (main_thread_) {
      main_thread_->SetTaskReloader(nullptr);
      main_thread_->SetQueueEmptyNotifyTarget(nullptr);
      delete main_thread_;
      main_thread_ = nullptr;
    }

    if (worker_thread_) {
      worker_thread_->SetNextInLine(nullptr);
      delete worker_thread_;
      worker_thread_ = nullptr;
    }
  }

  void CheckAndDestroyThreads() {
    if (!abort_destroy_threads_) {
      DestroyThreads();
    }
    abort_destroy_threads_ = false;
  }

  class WorkerThread*               worker_thread_;
  class MainThread*                 main_thread_;
  std::shared_ptr<AsyncTaskQueue>   queue_;
  bool                              abort_destroy_threads_;
};

AsyncTaskRunner::AsyncTaskRunner() {
  d_ = new AsyncTaskRunnerD();
}

AsyncTaskRunner::~AsyncTaskRunner() {
  delete d_;
}

void AsyncTaskRunner::PostTask(AsyncTask* task,
    AsyncTaskPayload* payload, const char* msgTxt) {
  task->SetPayload(payload);
  d_->queue_->AddTask(task);
  d_->SendAsyncNotification(task, msgTxt);
}

v8_promise_t AsyncTaskRunner::PostPromiseTask(PromiseTask* task,
    AsyncTaskPayload* payload, const char* msgTxt) {
  task->SetMicrotaskRunner(new PromiseMicrotaskRunnerImpl());
  task->SetPayload(payload);
  d_->queue_->AddTask(task);
  d_->SendAsyncNotification(task, msgTxt);
  return task->CreatePromise();
}

v8_promise_t AsyncTaskRunner::PostEventEmitterTask(EventEmitterTask* task,
    AsyncTaskPayload* payload, v8_object_t object, const char* msgTxt) {
  task->SetMicrotaskRunner(new PromiseMicrotaskRunnerImpl());
  task->SetPayload(payload);
  task->SetJavaScriptThis(object);
  d_->queue_->AddTask(task);
  d_->SendAsyncNotification(task, msgTxt);
  return task->CreatePromise();
}

v8_promise_t AsyncTaskRunner::PostBusyWaitingTask(BusyWaitingTask* task,
    AsyncTaskPayload* payload, const char* msgTxt) {
  task->SetMicrotaskRunner(new PromiseMicrotaskRunnerImpl());
  task->SetPayload(payload);
  d_->queue_->AddTask(task);
  d_->SendAsyncNotification(task, msgTxt);
  return task->CreatePromise();
}

v8_promise_t AsyncTaskRunner::RequestPauseTask(const std::string& task_id) {
  d_->queue_->RequestPauseTask(task_id);
  auto waiting_task = new BusyWaitingPauseTask(&(*d_->queue_), task_id);
  return PostBusyWaitingTask(waiting_task, nullptr,
    "{{INTERNAL_WAITING_FOR_PAUSE_TASK}}");
}

v8_promise_t AsyncTaskRunner::RequestResumeTask(const std::string& task_id) {
  d_->queue_->RequestResumeTask(task_id);
  auto waiting_task = new BusyWaitingResumeTask(&(*d_->queue_), task_id);
  return PostBusyWaitingTask(waiting_task, nullptr,
    "{{INTERNAL_WAITING_FOR_RESUME_TASK}}");
}

v8_promise_t AsyncTaskRunner::RequestCancelTask(const std::string& task_id) {
  d_->queue_->RequestCancelTask(task_id);
  if (d_->queue_->IsTaskPending(task_id)) {
    d_->queue_->ForceSetTaskState(task_id, AsyncTask::AsyncTaskState::Ready);
  }
  auto waiting_task = new BusyWaitingCanceledTask(&(*d_->queue_), task_id);
  return PostBusyWaitingTask(waiting_task, nullptr,
    "{{INTERNAL_WAITING_FOR_CANCEL_TASK}}");
}
