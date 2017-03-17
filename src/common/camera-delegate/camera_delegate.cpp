// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "camera_delegate.h"
#include <rs/playback/playback_context.h>
#include <string>
#include <vector>

static const size_t MAX_USB_DEVICES = 256;

/////////////////////////////////////////////////////////////////////////////

class CameraDelegateDeviceD {
 public:
  friend class CameraDelegate;
  friend class CameraDelegateDevice;

  CameraDelegateDeviceD() {
    device = nullptr;
    streaming_counter = 0;
    motion_handler_set = false;
  }

  rs::device* device;  // No need to delete this pointer
  int streaming_counter;

  typedef std::function<void(rs::motion_data)> motion_handler_t;
  typedef std::function<void(rs::timestamp_data)> timestamp_handler_t;

  std::vector<motion_handler_t> motion_handlers;
  std::vector<timestamp_handler_t> timestamp_handlers;

  bool motion_handler_set;
};

/////////////////////////////////////////////////////////////////////////////

class CameraDelegateD {
  CameraDelegateD() {
    context = nullptr;
    device_delegate.reserve(MAX_USB_DEVICES);  // Ensures no memory re-alloc
    device_delegate.resize(0);
  }

  friend class CameraDelegate;
  rs::core::context_interface* context;
  std::vector<CameraDelegateDeviceD> device_delegate;
  std::string playback_filename;

  void ResetContext() {
    delete context;
    context = nullptr;
  }
};

/////////////////////////////////////////////////////////////////////////////
CameraDelegate::CameraDelegate() {
  d_ = new CameraDelegateD();
  ConnectToRealCamera();
}

CameraDelegate::~CameraDelegate() {
  d_->ResetContext();
  delete d_;
}

void CameraDelegate::ConnectToVirtualCamera(const std::string& fileName) {
  d_->ResetContext();
  d_->context = new rs::playback::context(fileName.c_str());
  d_->playback_filename = fileName;
  for (size_t i = 0 ; i < d_->device_delegate.size() ; ++i) {
    d_->device_delegate[i].device = d_->context->get_device(i);
  }
}

void CameraDelegate::ConnectToRealCamera() {
  d_->ResetContext();
  d_->context = new rs::core::context();
  for (size_t i = 0 ; i < d_->device_delegate.size() ; ++i) {
    d_->device_delegate[i].device = d_->context->get_device(i);
  }
  d_->playback_filename = "";
}

bool CameraDelegate::IsVirtualCamera() const {
  return d_->playback_filename.length() > 0;
}

const std::string& CameraDelegate::GetVirtualCameraFileName() const {
  return d_->playback_filename;
}

int CameraDelegate::get_device_count() const {
  return d_->context->get_device_count();
}

CameraDelegateDevice* CameraDelegate::get_device(int index) {
  const size_t idx = index;
  if (idx >= MAX_USB_DEVICES) {
    return nullptr;
  }
  if (idx >= d_->device_delegate.size()) {
    d_->device_delegate.resize(idx + 1);
  }
  d_->device_delegate[idx].device = d_->context->get_device(index);
  return new CameraDelegateDevice(d_->device_delegate.data() + idx);
}

/////////////////////////////////////////////////////////////////////////////

CameraDelegateDevice::CameraDelegateDevice(CameraDelegateDeviceD* d) {
  d_ = d;
}

CameraDelegateDevice::~CameraDelegateDevice() {
}

rs::device* CameraDelegateDevice::get_device() {
  return d_->device;
}

const char* CameraDelegateDevice::get_name() const {
  return d_->device->get_name();
}

const char* CameraDelegateDevice::get_serial() const {
  return d_->device->get_serial();
}

const char* CameraDelegateDevice::get_usb_port_id() const {
  return d_->device->get_usb_port_id();
}

const char* CameraDelegateDevice::get_firmware_version() const {
  return d_->device->get_firmware_version();
}

const char* CameraDelegateDevice::get_info(rs::camera_info info) const {
  return d_->device->get_info(info);
}

rs::extrinsics CameraDelegateDevice::get_extrinsics(rs::stream from_stream,
    rs::stream to_stream) const {
  return d_->device->get_extrinsics(from_stream, to_stream);
}

rs::extrinsics CameraDelegateDevice::get_motion_extrinsics_from(
    rs::stream from_stream) const {
  return d_->device->get_motion_extrinsics_from(from_stream);
}

float CameraDelegateDevice::get_depth_scale() const {
  return d_->device->get_depth_scale();
}

bool CameraDelegateDevice::supports_option(rs::option option) const {
  return d_->device->supports_option(option);
}

int CameraDelegateDevice::get_stream_mode_count(rs::stream stream) const {
  return d_->device->get_stream_mode_count(stream);
}

void CameraDelegateDevice::get_stream_mode(rs::stream stream, int index,
    int& width, int& height, rs::format & format, int & framerate) const {
  return d_->device->get_stream_mode(stream, index, width, height,
    format, framerate);
}

void CameraDelegateDevice::enable_stream(rs::stream stream,
    int width, int height, rs::format format, int framerate,
    rs::output_buffer_format output_buffer_type) {
  if (!is_streaming()) {
    // Only when camera is not working
    return d_->device->enable_stream(stream, width, height,
        format, framerate, output_buffer_type);
  }
}

void CameraDelegateDevice::enable_stream(rs::stream stream,
    rs::preset preset) {
  if (!is_streaming()) {
    // Only when camera is not working
    return d_->device->enable_stream(stream, preset);
  }
}

void CameraDelegateDevice::disable_stream(rs::stream stream) {
  if (!is_streaming()) {
    // Only when camera is not working
    return d_->device->disable_stream(stream);
  }
}

bool CameraDelegateDevice::is_stream_enabled(rs::stream stream) const {
  return d_->device->is_stream_enabled(stream);
}

int CameraDelegateDevice::get_stream_width(rs::stream stream) const {
  return d_->device->get_stream_width(stream);
}

int CameraDelegateDevice::get_stream_height(rs::stream stream) const {
  return d_->device->get_stream_height(stream);
}

rs::format CameraDelegateDevice::get_stream_format(rs::stream stream) const {
  return d_->device->get_stream_format(stream);
}

int CameraDelegateDevice::get_stream_framerate(rs::stream stream) const {
  return d_->device->get_stream_framerate(stream);
}

rs::intrinsics CameraDelegateDevice::get_stream_intrinsics(
    rs::stream stream) const {
  return d_->device->get_stream_intrinsics(stream);
}

rs::motion_intrinsics CameraDelegateDevice::get_motion_intrinsics() const {
  return d_->device->get_motion_intrinsics();
}

void CameraDelegateDevice::set_frame_callback(rs::stream stream,
    std::function<void(rs::frame)> frame_handler) {
  return d_->device->set_frame_callback(stream, frame_handler);
}

void CameraDelegateDevice::enable_motion_tracking(
    std::function<void(rs::motion_data)> motion_handler,
    std::function<void(rs::timestamp_data)> timestamp_handler) {
  if (!d_->motion_handler_set) {
    d_->device->enable_motion_tracking([this](rs::motion_data data){
      for (auto i = this->d_->motion_handlers.begin();
        i != this->d_->motion_handlers.end();
        ++i) {
        i->operator () (data);
      }
    }, [this](rs::timestamp_data data){
      for (auto i = this->d_->timestamp_handlers.begin();
        i != this->d_->timestamp_handlers.end();
        ++i) {
        i->operator () (data);
      }
    });
    d_->motion_handler_set = true;
  }

  d_->motion_handlers.push_back(motion_handler);
  d_->timestamp_handlers.push_back(timestamp_handler);
}

void CameraDelegateDevice::enable_motion_tracking(
    std::function<void(rs::motion_data)> motion_handler) {
  if (!d_->motion_handler_set) {
    d_->device->enable_motion_tracking([this](rs::motion_data data){
      for (auto i = this->d_->motion_handlers.begin();
        i != this->d_->motion_handlers.end();
        ++i) {
        i->operator () (data);
      }
    });
    d_->motion_handler_set = true;
  }

  d_->motion_handlers.push_back(motion_handler);
}

void CameraDelegateDevice::disable_motion_tracking(void) {
  d_->motion_handler_set = false;
  d_->motion_handlers.resize(0);
  d_->timestamp_handlers.resize(0);

  return d_->device->disable_motion_tracking();
}

int CameraDelegateDevice::is_motion_tracking_active() {
  return d_->device->is_motion_tracking_active();
}

void CameraDelegateDevice::start(rs::source source) {
  ++d_->streaming_counter;
  if (!is_streaming()) {
    // Only call through for the first time
    d_->device->start(source);
  }
}

void CameraDelegateDevice::stop(rs::source source) {
  --d_->streaming_counter;

  if (d_->streaming_counter <= 0) {
    d_->streaming_counter = 0;
    // It's time to stop it for real
    return d_->device->stop(source);
  }
}

bool CameraDelegateDevice::is_streaming() const {
  return d_->device->is_streaming();
}

void CameraDelegateDevice::get_option_range(rs::option option,
    double & min, double & max, double & step) {
  return d_->device->get_option_range(option, min, max, step);
}

void CameraDelegateDevice::get_option_range(rs::option option,
    double & min, double & max, double & step, double & def) {
  return d_->device->get_option_range(option, min, max, step, def);
}

void CameraDelegateDevice::get_options(const rs::option* options,
    size_t count, double * values) {
  return d_->device->get_options(options, count, values);
}

void CameraDelegateDevice::set_options(const rs::option* options,
    size_t count, const double * values) {
  return d_->device->set_options(options, count, values);
}

double CameraDelegateDevice::get_option(rs::option option) {
  return d_->device->get_option(option);
}

const char* CameraDelegateDevice::get_option_description(rs::option option) {
  return d_->device->get_option_description(option);
}

void CameraDelegateDevice::set_option(rs::option option, double value) {
  return d_->device->set_option(option, value);
}

void CameraDelegateDevice::wait_for_frames() {
  return d_->device->wait_for_frames();
}

bool CameraDelegateDevice::poll_for_frames() {
  return d_->device->poll_for_frames();
}

bool CameraDelegateDevice::supports(rs::capabilities capability) const {
  return d_->device->supports(capability);
}

bool CameraDelegateDevice::supports(rs::camera_info info_param) const {
  return d_->device->supports(info_param);
}

double CameraDelegateDevice::get_frame_timestamp(rs::stream stream) const {
  return d_->device->get_frame_timestamp(stream);
}

unsigned long long CameraDelegateDevice::get_frame_number(  // NOLINT(*)
    rs::stream stream) const {
  return d_->device->get_frame_number(stream);
}

const void* CameraDelegateDevice::get_frame_data(rs::stream stream) const {
  return d_->device->get_frame_data(stream);
}

void CameraDelegateDevice::send_blob_to_device(rs::blob_type type,
    void * data, int size) {
  return d_->device->send_blob_to_device(type, data, size);
}
