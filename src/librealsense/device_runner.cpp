// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "device_runner.h"

#include <functional>

#include "common/task/async_task_runner_instance.h"
#include "gen/nan__frame_data.h"
#include "rs_extrinsics.h"
#include "rs_payload.h"
#include "rs_task.h"
#include "utils.h"

DeviceRunner::DeviceRunner(rs::device* device)
    : device_(device) {
}

DeviceRunner::~DeviceRunner() {
}

std::string DeviceRunner::GetName() const {
  return device_->get_name();
}

std::string DeviceRunner::GetSerial() const {
  return device_->get_serial();
}

std::string DeviceRunner::GetFirmwareVersion() const {
  return device_->get_firmware_version();
}

double DeviceRunner::GetDepthScale() const {
  return device_->get_depth_scale();
}

bool DeviceRunner::IsStreaming() const {
  return device_->is_streaming();
}

void DeviceRunner::Start() {
  device_->start();
}

void DeviceRunner::Stop() {
  device_->stop();
}

bool DeviceRunner::SupportsOption(const std::string& option) {
  return device_->supports_option(GetRSOptionFromString(option));
}

int DeviceRunner::GetStreamWidth(const std::string& stream) {
  return device_->get_stream_width(GetRSStreamFromString(stream));
}

int DeviceRunner::GetStreamHeight(const std::string& stream) {
  return device_->get_stream_height(GetRSStreamFromString(stream));
}

int DeviceRunner::GetFrameTimeStamp(const std::string& stream) {
  return device_->get_frame_timestamp(GetRSStreamFromString(stream));
}

bool DeviceRunner::IsStreamEnabled(const std::string& stream) {
  return device_->is_stream_enabled(GetRSStreamFromString(stream));
}

void DeviceRunner::EnableStream(const std::string& stream,
                                const DictionaryMode& mode) {
  rs::stream rs_stream = GetRSStreamFromString(stream);

  device_->enable_stream(rs_stream, mode.member_width,
      mode.member_height, GetRSFormatFromString(mode.member_format),
      mode.member_framerate);
  device_->set_frame_callback(
      rs_stream, std::bind(&DeviceRunner::GetFrameDataFromRSFrame, this,
                           std::placeholders::_1));

  enabled_streams_vector_.push_back(rs_stream);
}

int DeviceRunner::GetStreamModeCount(const std::string& stream) {
  return device_->get_stream_mode_count(GetRSStreamFromString(stream));
}

DictionaryMode DeviceRunner::GetStreamMode(const std::string& stream,
                                           int index) {
  int width, height, framerate;
  rs::format format;

  device_->get_stream_mode(GetRSStreamFromString(stream), index, width, height,
      format, framerate);
  std::string formatStr = GetStringFromRSFormat(format);

  DictionaryMode mode;
  mode.has_member_width = true;
  mode.member_width = width;
  mode.has_member_height = true;
  mode.member_height = height;
  mode.has_member_format = true;
  mode.member_format = GetStringFromRSFormat(format);
  mode.has_member_framerate = true;
  mode.member_framerate = framerate;

  return mode;
}

std::string DeviceRunner::GetUSBPortId() {
  return device_->get_usb_port_id();
}

std::string DeviceRunner::GetInfo(const std::string& info) {
  rs::camera_info rs_camera_info = GetCameraInfoFromString(info);
  return device_->get_info(rs_camera_info);
}

RSExtrinsics* DeviceRunner::GetExtrinsics(const std::string& from,
                                       const std::string& to) {
  rs::stream from_stream = GetRSStreamFromString(from);
  rs::stream to_stream = GetRSStreamFromString(to);
  rs::extrinsics rs_extrinsics =
      device_->get_extrinsics(from_stream, to_stream);

  return new RSExtrinsics(rs_extrinsics);
}

RSExtrinsics* DeviceRunner::GetMotionExtrinsicsFrom(const std::string& from) {
  rs::stream from_stream = GetRSStreamFromString(from);
  rs::extrinsics rs_extrinsics =
    device_->get_motion_extrinsics_from(from_stream);

  return new RSExtrinsics(rs_extrinsics);
}

void DeviceRunner::DisableStream(const std::string& stream) {
  rs::stream rs_stream = GetRSStreamFromString(stream);
  std::vector<rs::stream>::iterator it =
      std::find(enabled_streams_vector_.begin(),
                enabled_streams_vector_.end(),
                rs_stream);

  if (it != enabled_streams_vector_.end()) {
    device_->disable_stream(rs_stream);
    enabled_streams_vector_.erase(it);
  }
}

std::string DeviceRunner::GetStreamFormat(const std::string& stream) {
  rs::stream rs_stream = GetRSStreamFromString(stream);
  return GetStringFromRSFormat(device_->get_stream_format(rs_stream));
}

int DeviceRunner::GetStreamFramerate(const std::string& stream) {
  rs::stream rs_stream = GetRSStreamFromString(stream);
  return device_->get_stream_framerate(rs_stream);
}

int DeviceRunner::GetFrameNumber(const std::string& stream) {
  rs::stream rs_stream = GetRSStreamFromString(stream);
  return device_->get_frame_number(rs_stream);
}

void DeviceRunner::GetFrameDataFromRSFrame(rs::frame frame) {
  int width = frame.get_width();
  int height = frame.get_height();
  int frame_size = width * height * frame.get_bpp() / 8;
  char* data = static_cast<char*>(malloc(frame_size));

  memcpy(data, frame.get_data(), frame_size);

  FrameData* frame_data = new FrameData();
  ArrayBuffer array_buffer;
  array_buffer.data = data;
  array_buffer.size = frame_size;

  frame_data->stream_ = GetStringFromRSStream(frame.get_stream_type());
  frame_data->data_ = array_buffer;

  auto payload = new RSPayload<FrameData*>;
  payload->SetPayloadData(frame_data);

  AsyncTaskRunnerInstance::GetInstance()->PostTask(
      new RSFrameReadyEventTask(&js_this_),
      payload,
      "{{FRAME_READY MESSAGE}}");
}
