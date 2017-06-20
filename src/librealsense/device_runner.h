// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _DEVICE_RUNNER_H_
#define _DEVICE_RUNNER_H_

#include <librealsense/rs.hpp>
#include <string>
#include <vector>

#include "frame_data.h"
#include "gen/mode.h"
#include "rs_extrinsics.h"

namespace rs {
class device;
}  // namespace rs

class RSExtrinsics;

class DeviceRunner {
 public:
  explicit DeviceRunner(rs::device* device);
  ~DeviceRunner();

  std::string GetName() const;
  std::string GetSerial() const;
  std::string GetFirmwareVersion() const;
  double GetDepthScale() const;
  bool IsStreaming() const;

  bool SupportsOption(const std::string& option);
  int GetStreamModeCount(const std::string& stream);
  DictionaryMode GetStreamMode(const std::string& steam, int index);
  bool IsStreamEnabled(const std::string& stream);
  void EnableStream(const std::string& stream, const DictionaryMode& mode);
  int GetStreamWidth(const std::string& stream);
  int GetStreamHeight(const std::string& stream);
  int GetFrameTimeStamp(const std::string& stream);

  void GetFrameDataFromRSFrame(rs::frame frame);
  std::string GetUSBPortId();
  std::string GetInfo(const std::string& info);
  RSExtrinsics* GetExtrinsics(const std::string& from, const std::string& to);
  RSExtrinsics* GetMotionExtrinsicsFrom(const std::string& from);
  void DisableStream(const std::string& stream);
  std::string GetStreamFormat(const std::string& stream);
  int GetStreamFramerate(const std::string& stream);
  int GetFrameNumber(const std::string& stream);

  void Start();
  void Stop();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    js_this_.Reset(obj);
  }

 private:
  rs::device* device_;
  std::vector<rs::stream> enabled_streams_vector_;
  Nan::Persistent<v8::Object> js_this_;
};

#endif  // #define _DEVICE_RUNNER_H_
