// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/slam_worker.h"

SlamWorker::SlamWorker() {
  state_ = SlamWorkerState::IDLE;
  uv_sem_init(&req_semaphore_, 0);
  uv_mutex_init(&request_mutex_);
  uv_mutex_init(&state_mutex_);
}

bool SlamWorker::Start() {
  uv_mutex_lock(&state_mutex_);
  if (state_ != SlamWorkerState::IDLE) {
    uv_mutex_unlock(&state_mutex_);
    return false;
  }

  if (0 != uv_thread_create(&thread_, WorkingLoop, this)) {
    uv_mutex_unlock(&state_mutex_);
    return false;
  }
  state_ = SlamWorkerState::RUNNING;
  uv_mutex_unlock(&state_mutex_);

  return true;
}

void SlamWorker::Stop() {
  uv_mutex_lock(&request_mutex_);
  while (!requests_.empty()) {
    auto req = requests_.front();
    requests_.pop();
    delete req;
  }
  uv_mutex_unlock(&request_mutex_);

  uv_mutex_lock(&state_mutex_);
  if (state_ == SlamWorkerState::IDLE) {
    uv_mutex_unlock(&state_mutex_);
    return;
  } else if (state_ == SlamWorkerState::RUNNING) {
    state_ = SlamWorkerState::STOP;
  }
  uv_mutex_unlock(&state_mutex_);

  if (0 != uv_thread_join(&thread_)) {
    std::cout << "Error: failed to join uv thread." << std::endl;
  }
}

SlamWorker::~SlamWorker() {
  Stop();

  uv_sem_destroy(&req_semaphore_);
  uv_mutex_destroy(&request_mutex_);
  uv_mutex_destroy(&state_mutex_);
}

// Static function.
// Consume the requests.
void SlamWorker::WorkingLoop(void* arg) {
  SlamWorker* me = reinterpret_cast<SlamWorker*>(arg);

  while (1) {
    uv_mutex_lock(&me->state_mutex_);
    if (me->state_ != SlamWorkerState::RUNNING) {
      me->state_ = SlamWorkerState::IDLE;
      uv_mutex_unlock(&me->state_mutex_);
      return;
    }

    // 1. pop out a request.
    uv_mutex_lock(&me->request_mutex_);
    if (0 != uv_sem_trywait(&me->req_semaphore_)) {
      // No valid request in the queue.
      uv_mutex_unlock(&me->request_mutex_);
      uv_mutex_unlock(&me->state_mutex_);
      continue;
    }
    auto req = me->requests_.front();
    me->requests_.pop();
    uv_mutex_unlock(&me->request_mutex_);

    // 2. do the requested work.
    req->DoWork();

    // 3. send async message to main thread with the consumed request object.
    auto message = new AsyncMessage(
        AsyncMessageType::kRequestDone, static_cast<void*>(req), me->channel_);
    me->channel_->SendNotice(message);
    uv_mutex_unlock(&me->state_mutex_);
  }
}

// Put in the requests.
void SlamWorker::PostRequest(AsyncRequest* req) {
  uv_mutex_lock(&request_mutex_);
  requests_.push(req);
  uv_sem_post(&req_semaphore_);
  uv_mutex_unlock(&request_mutex_);
  return;
}
