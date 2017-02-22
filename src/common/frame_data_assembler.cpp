// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "frame_data_assembler.h"

#include <string>

FrameDataAssembler::~FrameDataAssembler() {
  if (fisheye_ != nullptr) delete fisheye_;

  if (depth_ != nullptr) delete depth_;

  if (color_ != nullptr) delete color_;

  if (gyro_ != nullptr) delete gyro_;

  if (accel_ != nullptr) delete accel_;
}

void FrameDataAssembler::SetData(rs::stream stream, int height,
    int width, rs::format format, double timestamp,
    uint64_t frame_number, const char* data) {
  if (!data)
    return;
  switch (stream) {
    case rs::stream::color:
      if (color_ == nullptr) color_ = new StreamFrameData();

      color_->SetData(stream, height, width, format, timestamp,
          frame_number, data);
      break;
    case rs::stream::depth:
      if (depth_ == nullptr) depth_ = new StreamFrameData();

      depth_->SetData(stream, height, width, format, timestamp,
          frame_number, data);
      break;
    case rs::stream::fisheye:
      if (fisheye_ == nullptr) fisheye_ = new StreamFrameData();

      fisheye_->SetData(stream, height, width, format, timestamp,
          frame_number, data);
      break;
    default:
      // TODO(Donna): log out error.
      return;
  }
}

void FrameDataAssembler::SetData(rs::core::image_interface* image) {
  if (!image || !image->query_data())
    return;
  switch (image->query_stream_type()) {
    case rs::core::stream_type::color:
      if (color_ == nullptr) color_ = new StreamFrameData();

      color_->SetData(image);
      break;
    case rs::core::stream_type::depth:
      if (depth_ == nullptr) depth_ = new StreamFrameData();

      depth_->SetData(image);
      break;
    case rs::core::stream_type::fisheye:
      if (fisheye_ == nullptr) fisheye_ = new StreamFrameData();

      fisheye_->SetData(image);
      break;
    default:
      // TODO(Donna): log out error.
      return;
  }
}

void FrameDataAssembler::SetData(rs::event motionType,
    double timestamp, uint64_t frame_number, const float* data) {
  if (!data)
    return;
  switch (motionType) {
    case rs::event::event_imu_accel:
      if (accel_ == nullptr) accel_ = new MotionData();

      accel_->SetData(motionType, timestamp, frame_number, data);
      break;
    case rs::event::event_imu_gyro:
      if (gyro_ == nullptr) gyro_ = new MotionData();

      gyro_->SetData(motionType, timestamp, frame_number, data);
      break;
    default:
      // TODO(Donna): log out error.
      return;
  }
}

void FrameDataAssembler::SetData(const rs::core::motion_sample& sample) {
  switch (sample.type) {
    case rs::core::motion_type::accel:
      if (accel_ == nullptr) accel_ = new MotionData();

      accel_->SetData(sample);
      break;
    case rs::core::motion_type::gyro:
      if (gyro_ == nullptr) gyro_ = new MotionData();

      gyro_->SetData(sample);
      break;
    default:
      // TODO(Donna): log out error.
      return;
  }
}

FrameData* FrameDataAssembler::ToFrameData() {
  FrameData* frameData = new FrameData;
  if (color_ != nullptr) {
    auto img = color_->ToImage();
    if (img) {
      frameData->set_color(img);
      delete img;
    }
  }

  if (depth_ != nullptr) {
    auto img = depth_->ToImage();
    if (img) {
      frameData->set_depth(img);
      delete img;
    }
  }

  if (fisheye_ != nullptr) {
    auto img = fisheye_->ToImage();
    if (img) {
      frameData->set_fisheye(img);
      delete img;
    }
  }

  if (gyro_ != nullptr) {
    auto sample = gyro_->ToMotionSample();
    if (sample) {
      frameData->set_gyro(sample);
      delete sample;
    }
  }

  if (accel_ != nullptr) {
    auto sample = accel_->ToMotionSample();
    if (sample) {
      frameData->set_accel(sample);
      delete sample;
    }
  }

  return frameData;
}

std::string FrameDataAssembler::GetFormatString(rs::format format) {
  switch (format) {
    case rs::format::any:
      return "any";
    case rs::format::z16:
      return "z16";
    case rs::format::disparity16:
      return "disparity16";
    case rs::format::xyz32f:
      return "xyz32f";
    case rs::format::yuyv:
      return "yuyv";
    case rs::format::rgb8:
      return "rgb8";
    case rs::format::bgr8:
      return "bgr8";
    case rs::format::rgba8:
      return "rgba8";
    case rs::format::bgra8:
      return "bgra8";
    case rs::format::y8:
      return "y8";
    case rs::format::y16:
      return "y16";
    case rs::format::raw10:
      return "raw10";
    case rs::format::raw16:
      return "raw16";
    case rs::format::raw8:
      return "raw8";
    default:
      return "not-supported-format";
  }
}
