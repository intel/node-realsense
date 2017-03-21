// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_DELEGATE_H_
#define _CAMERA_DELEGATE_H_

#include <librealsense/rs.hpp>
#include <string>

class CameraDelegateDevice {
  CameraDelegateDevice(const CameraDelegateDevice&) = delete;
  CameraDelegateDevice& operator = (const CameraDelegateDevice&) = delete;

 public:
  explicit CameraDelegateDevice(class CameraDelegateDeviceD*);
  ~CameraDelegateDevice();

 public:
  rs::device* get_device();

  const char * get_name() const;
  const char * get_serial() const;
  const char * get_usb_port_id() const;
  const char * get_firmware_version() const;
  const char * get_info(rs::camera_info info) const;
  rs::extrinsics get_extrinsics(rs::stream from_stream,
      rs::stream to_stream) const;
  rs::extrinsics get_motion_extrinsics_from(rs::stream from_stream) const;
  float get_depth_scale() const;
  bool supports_option(rs::option option) const;
  int get_stream_mode_count(rs::stream stream) const;
  void get_stream_mode(rs::stream stream, int index,
      int & width, int & height, rs::format & format, int & framerate) const;  // NOLINT(*)
  void enable_stream(rs::stream stream, int width, int height,
      rs::format format, int framerate,
      rs::output_buffer_format output_buffer_type
      = rs::output_buffer_format::continous);
  void enable_stream(rs::stream stream, rs::preset preset);
  void disable_stream(rs::stream stream);
  bool is_stream_enabled(rs::stream stream) const;
  int get_stream_width(rs::stream stream) const;
  int get_stream_height(rs::stream stream) const;
  rs::format get_stream_format(rs::stream stream) const;
  int get_stream_framerate(rs::stream stream) const;
  rs::intrinsics get_stream_intrinsics(rs::stream stream) const;
  rs::motion_intrinsics get_motion_intrinsics() const;
  void set_frame_callback(
      rs::stream stream, std::function<void(rs::frame)> frame_handler);
  void enable_motion_tracking(
      std::function<void(rs::motion_data)> motion_handler,
      std::function<void(rs::timestamp_data)> timestamp_handler);
  void enable_motion_tracking(
      std::function<void(rs::motion_data)> motion_handler);
  void disable_motion_tracking(void);
  int is_motion_tracking_active();
  void start(rs::source source = rs::source::video);
  void stop(rs::source source = rs::source::video);
  bool is_streaming() const;
  void get_option_range(rs::option option,
      double & min, double & max, double & step);  // NOLINT(*)
  void get_option_range(rs::option option,
      double & min, double & max, double & step, double & def);  // NOLINT(*)
  void get_options(const rs::option* options,
      size_t count, double * values);
  void set_options(const rs::option* options,
      size_t count, const double * values);
  double get_option(rs::option option);
  const char * get_option_description(rs::option option);
  void set_option(rs::option option, double value);
  void wait_for_frames();
  bool poll_for_frames();
  bool supports(rs::capabilities capability) const;
  bool supports(rs::camera_info info_param) const;
  double get_frame_timestamp(rs::stream stream) const;
  unsigned long long get_frame_number(rs::stream stream) const;    // NOLINT(*)
  const void * get_frame_data(rs::stream stream) const;
  void send_blob_to_device(rs::blob_type type, void * data, int size);

 private:
  class CameraDelegateDeviceD * d_;
};

class CameraDelegate {
  CameraDelegate(const CameraDelegate&) = delete;
  CameraDelegate& operator = (const CameraDelegate&) = delete;

 public:
  CameraDelegate();
  ~CameraDelegate();

  void ConnectToVirtualCamera(const std::string& fileName);
  void ConnectToRealCamera();
  bool IsVirtualCamera() const;
  const std::string& GetVirtualCameraFileName() const;

 public:
  int get_device_count() const;
  CameraDelegateDevice* get_device(int index);

 private:
  class CameraDelegateD * d_;
};

#endif  // _CAMERA_DELEGATE_H_
