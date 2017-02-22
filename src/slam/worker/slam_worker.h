// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_SLAM_WORKER_H_
#define _WORKER_SLAM_WORKER_H_

#include <uv.h>

#include <memory>
#include <queue>

#include "async_channel.h"
#include "worker/async_request.h"

// IDLE --Start()-> RUNNING --Stop()-> STOP
enum class SlamWorkerState {
  IDLE = 0,
  RUNNING = 1,
  STOP = 2
};

class SlamWorker {
 public:
  SlamWorker();
  ~SlamWorker();

  bool Start();
  void Stop();

  void PostRequest(AsyncRequest* req);

  void set_channel(std::shared_ptr<AsyncChannel> channel) {
    channel_ = channel;
  }

 private:
  // The working loop will keep going to fetch and consume
  // request from request queue until the queue is empty.
  // When the request queue empty, the thread will sleep until
  // a coming request to wake it up.
  static void WorkingLoop(void* arg);

  uv_mutex_t state_mutex_;
  SlamWorkerState state_;
  uv_thread_t thread_;
  std::shared_ptr<AsyncChannel> channel_;

  // Protect the request queue and req_semaphore_.
  uv_mutex_t request_mutex_;
  uv_sem_t req_semaphore_;

  std::queue<AsyncRequest*> requests_;
};

#endif  // _WORKER_SLAM_WORKER_H_
