// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "async_channel.h"

AsyncChannel::~AsyncChannel() {
  async_ = nullptr;
  emitter_.reset();
}

AsyncChannel::AsyncChannel(uv_async_t* async,
                           std::shared_ptr<EventEmitter> emitter,
                           bool tracking_event_enabled,
                           bool error_event_enabled)
    : async_(async), emitter_(emitter),
      tracking_event_enabled_(tracking_event_enabled),
      error_event_enabled_(error_event_enabled) {}

AsyncChannel& AsyncChannel::operator=(const AsyncChannel& rhs) {
  if (&rhs != this) {
    async_ = rhs.async_;
    emitter_ = rhs.emitter_;
  }
  return *this;
}

void AsyncChannel::SendNotice(AsyncMessage* msg) {
  async_->data = msg;
  uv_async_send(async_);
}

void AsyncChannel::emitEvent(int argc, v8::Local<v8::Value>* argv) const {
  emitter_->emit(argc, argv);
}
