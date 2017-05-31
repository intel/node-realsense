// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _DEVICE_H_
#define _DEVICE_H_

#include <node.h>
#include <v8.h>

#include <memory>
#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"
#include "gen/mode.h"

class DeviceRunner;

class Device {
 public:
  Device();

  Device(const Device& rhs);

  ~Device();

  Device& operator = (const Device& rhs);

 public:
  v8::Handle<v8::Promise> getName();

  v8::Handle<v8::Promise> getSerial();

  v8::Handle<v8::Promise> getFirmwareVersion();

  v8::Handle<v8::Promise> getDepthScale();

  v8::Handle<v8::Promise> isStreaming();

  v8::Handle<v8::Promise> supportsOption(const std::string& option);

  v8::Handle<v8::Promise> getStreamModeCount(const std::string& stream);

  v8::Handle<v8::Promise> getStreamMode(const std::string& stream,
                                        const int32_t& index);

  v8::Handle<v8::Promise> isStreamEnabled(const std::string& stream);

  v8::Handle<v8::Promise> enableStream(const std::string& stream,
                                       const Mode& mode);

  v8::Handle<v8::Promise> getStreamWidth(const std::string& stream);

  v8::Handle<v8::Promise> getStreamHeight(const std::string& stream);

  v8::Handle<v8::Promise> getFrameTimeStamp(const std::string& stream);

  v8::Handle<v8::Promise> getUSBPortId();

  v8::Handle<v8::Promise> getInfo(const std::string& info);

  v8::Handle<v8::Promise> getExtrinsics(const std::string& from,
                                        const std::string& to);

  v8::Handle<v8::Promise> getMotionExtrinsicsFrom(const std::string& from);

  v8::Handle<v8::Promise> disableStream(const std::string& stream);

  v8::Handle<v8::Promise> getStreamFormat(const std::string& stream);

  v8::Handle<v8::Promise> getStreamFramerate(const std::string& stream);

  v8::Handle<v8::Promise> getFrameNumber(const std::string& stream);

  v8::Handle<v8::Promise> start();

  v8::Handle<v8::Promise> stop();

  void SetJavaScriptThis(v8::Local<v8::Object> obj);

  void SetDeviceRunner(DeviceRunner* runner);

 private:
  std::unique_ptr<DeviceRunner> device_runner_;
  Nan::Persistent<v8::Object> js_this_;
};

#endif  // _DEVICE_H_
