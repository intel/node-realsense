// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "device.h"

#include <functional>

#include "common/task/async_task_runner_instance.h"
#include "device_runner.h"
#include "gen/mode.h"
#include "rs_payload.h"
#include "rs_task.h"

#define POST_DEVICE_PROMISE_TASK(type, method, message, ...) \
auto payload = new RSPayload<type>(); \
return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask( \
    new RSPromiseTask<type>( \
        std::bind(&DeviceRunner::method, device_runner_.get(), \
                  ##__VA_ARGS__)), \
    payload, \
    #message); \

Device::Device()
    : device_runner_(nullptr) {
}

Device::Device(const Device& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in device.h
}

Device::~Device() {
  // TODO(widl-nan): do cleanup if necessary
}

Device& Device::operator = (const Device& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

v8::Handle<v8::Promise> Device::getName() {
  POST_DEVICE_PROMISE_TASK(std::string, GetName, {{GET_NAME MESSAGE}})
}

v8::Handle<v8::Promise> Device::getSerial() {
  POST_DEVICE_PROMISE_TASK(std::string, GetSerial, {{GET_SERIAL MESSAGE}});
}

v8::Handle<v8::Promise> Device::getFirmwareVersion() {
  POST_DEVICE_PROMISE_TASK(std::string, GetFirmwareVersion,
      {{GET_FRIMWARE_VERSION MESSAGE}});
}

v8::Handle<v8::Promise> Device::getDepthScale() {
  POST_DEVICE_PROMISE_TASK(double, GetDepthScale, {{GET_DEPTH_SCALE MESSAGE}});
}

v8::Handle<v8::Promise> Device::isStreaming() {
  POST_DEVICE_PROMISE_TASK(bool, IsStreaming, {{GET_DEPTH_SCALE MESSAGE}});
}

v8::Handle<v8::Promise> Device::supportsOption(const std::string& option) {
  POST_DEVICE_PROMISE_TASK(bool, SupportsOption, {{GET_OPTION_SUPPORT MESSAGE}},
      option);
}

v8::Handle<v8::Promise> Device::getStreamModeCount(const std::string& stream) {
  POST_DEVICE_PROMISE_TASK(int, GetStreamModeCount,
      {{GET_STREAM_MODE_COUNT MESSAGE}}, stream);
}

v8::Handle<v8::Promise> Device::getStreamMode(const std::string& stream,
                                              const int32_t& index) {
  POST_DEVICE_PROMISE_TASK(DictionaryMode, GetStreamMode,
      {{GET_STREAM_MODE MESSAGE}}, stream, index);}

v8::Handle<v8::Promise> Device::isStreamEnabled(const std::string& stream) {
  POST_DEVICE_PROMISE_TASK(bool, IsStreamEnabled, {{GET_STREAM_ENABLE MESSAGE}},
      stream);
}

v8::Handle<v8::Promise> Device::enableStream(const std::string& stream,
                                             const Mode& mode) {
  DictionaryMode dictMode(mode);
  POST_DEVICE_PROMISE_TASK(void, EnableStream, {{ENABLE_STREAM MESSAGE}},
      stream, dictMode);
}

v8::Handle<v8::Promise> Device::getStreamWidth(const std::string& stream) {
  POST_DEVICE_PROMISE_TASK(int, GetStreamWidth, {{GET_STREAM_WIDTH MESSAGE}},
      stream);
}

v8::Handle<v8::Promise> Device::getStreamHeight(const std::string& stream) {
  POST_DEVICE_PROMISE_TASK(int, GetStreamHeight, {{GET_STREAM_HEIGHT MESSAGE}},
      stream);
}

v8::Handle<v8::Promise> Device::getFrameTimeStamp(const std::string& stream) {
  POST_DEVICE_PROMISE_TASK(int, GetFrameTimeStamp,
      {{GET_FRAME_TIMESTAMP MESSAGE}}, stream);
}

v8::Handle<v8::Promise> Device::start() {
  POST_DEVICE_PROMISE_TASK(void, Start, {{START_DEVICE MESSAGE}});
}

v8::Handle<v8::Promise> Device::stop() {
  POST_DEVICE_PROMISE_TASK(void, Stop, {{STOP_DEVICE MESSAGE}});
}

void Device::SetDeviceRunner(DeviceRunner* runner) {
  device_runner_.reset(runner);
  runner->SetJavaScriptThis(Nan::New(js_this_));
}

void Device::SetJavaScriptThis(v8::Local<v8::Object> obj) {
  js_this_.Reset(obj);
  device_runner_->SetJavaScriptThis(obj);
}
