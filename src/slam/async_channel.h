// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _ASYNC_CHANNEL_H_
#define _ASYNC_CHANNEL_H_

#include <nan.h>
#include <node.h>
#include <uv.h>

#include <iostream>
#include <memory>

enum class AsyncMessageType {
  kRequestDone = 0,
  kProcessFrameDone = 1
};

class EventEmitter {
 public:
  explicit EventEmitter(Nan::Persistent<v8::Object>* js_this)
      : js_this_(js_this) {}

  void emit(int argc, v8::Local<v8::Value>* argv) const {
    if (js_this_ == nullptr) {
      std::cout << "null js_this_ in when emit event." << std::endl;
      return;
    }
    v8::Local<v8::Object> js_this = Nan::New(*js_this_);
    Nan::MakeCallback(js_this, "emit", argc, argv);
  }

 private:
  Nan::Persistent<v8::Object>* js_this_;
};

class AsyncMessage;
class AsyncChannel {
 public:
  AsyncChannel();
  AsyncChannel(uv_async_t* async, std::shared_ptr<EventEmitter> emitter,
               bool tracking_event_enabled, bool error_event_enabled);
  ~AsyncChannel();

  AsyncChannel& operator=(const AsyncChannel& rhs);

  void SendNotice(AsyncMessage* msg);
  void emitEvent(int argc, v8::Local<v8::Value>* argv) const;

  bool tracking_event_enabled() const {
    return this->tracking_event_enabled_;
  }

  bool error_event_enabled() const {
    return this->error_event_enabled_;
  }

  void set_emitter(std::shared_ptr<EventEmitter> emitter) {
    emitter_ = emitter;
  }

  void set_tracking_event_enabled(bool newValue) {
    this->tracking_event_enabled_ = newValue;
  }

  void set_error_event_enabled(bool newValue) {
    this->error_event_enabled_ = newValue;
  }

 private:
  uv_async_t* async_;
  std::shared_ptr<EventEmitter> emitter_;
  bool tracking_event_enabled_;
  bool error_event_enabled_;
};

class AsyncMessage {
 public:
  AsyncMessage(AsyncMessageType type, void* data)
      : type_(type), data_(data) {
    channel_.reset();
  }

  AsyncMessage(AsyncMessageType type, void* data,
               std::shared_ptr<AsyncChannel> channel)
      : type_(type), data_(data), channel_(channel) {}

  void set_channel(std::shared_ptr<AsyncChannel> channel) {
    channel_ = channel;
  }

  AsyncMessageType type() {
    return type_;
  }
  void* data() {
    return data_;
  }
  std::shared_ptr<AsyncChannel> channel() {
    return channel_;
  }

 private:
  AsyncMessageType type_;
  void* data_;
  std::shared_ptr<AsyncChannel> channel_;
};

#endif  // _ASYNC_CHANNEL_H_
