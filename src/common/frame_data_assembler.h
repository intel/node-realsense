// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_FRAME_DATA_ASSEMBLER_H
#define _COMMON_FRAME_DATA_ASSEMBLER_H

#include <librealsense/rs.hpp>
#include <rs_core.h>
#include <rs_utils.h>

#include <string>
#include <iostream>

#include "frame_data.h"
#include "image_utils.h"

class FrameDataAssembler {
 public:
  class StreamFrameData  {
   public:
    StreamFrameData()
        : width_(0), height_(0), data_(nullptr), size_(0) {}

    void SetData(rs::stream stream, int width, int height, rs::format format,
        double timestamp, uint64_t frame_number, const char* data) {
      if (!data)
        return;
      auto format_size = image_utils::GetPixelSize(format);

      if (width <= 0 || height <= 0 || format_size <= 0) return;
      // TODO(Donna): log out a invalid parameter message.

      int32_t size = format_size * width * height;

      if (size <= 0) return;

      stream_ = stream;
      format_ = format;
      width_ = width;
      height_ = height;
      frame_number_ = frame_number;
      timestamp_ = timestamp;

      // The input buf is owned by mw, so we need to make a copy.
      if (data_ == nullptr) {
        data_ = new char[size];
      } else if (size_ != size) {
        delete [] data_;
        data_ = new char[size];
      }
      memcpy(data_, data, size);
      size_ = size;
    }

    void SetData(rs::core::image_interface* image) {
      auto stream = rs::utils::convert_stream_type(image->query_stream_type());
      auto imageInfo = image->query_info();
      width_ = imageInfo.width;
      height_ = imageInfo.height;
      SetData(stream, imageInfo.width, imageInfo.height,
              rs::utils::convert_pixel_format(imageInfo.format),
              image->query_time_stamp(),
              image->query_frame_number(),
              static_cast<const char*>(image->query_data()));
    }

    ~StreamFrameData() {
      if (data_ != nullptr) delete [] data_;
    }

    Image* ToImage() {
      if (data_ == nullptr) return nullptr;

      Image* img = new Image;
      img->set_width(width_);
      img->set_height(height_);
      img->set_timeStamp(timestamp_);
      img->set_frameNumber(frame_number_);
      img->set_format(GetFormatString(format_));
      ArrayBuffer buf = {data_, size_};
      img->set_data(buf);

      // TODO(Donna): ArrayBuffer will own the buffer, so clear the
      // data_ to avoid multiply free. We should remove following two
      // lines if the ownership mechanism changed.
      data_ = nullptr;
      size_ = 0;

      return img;
    }

   private:
    rs::stream stream_;
    int width_;
    int height_;
    uint64_t frame_number_;
    double timestamp_;
    rs::format format_;
    char* data_;
    int32_t size_;
    friend class FrameDataAssembler;
  };

  class MotionData  {
   public:
    MotionData() : data_(new float[3]), size_(3) {}

    ~MotionData() {
      if (data_ != nullptr) delete [] data_;
    }

    void SetData(rs::event motionType, uint64_t frame_number,
                 double timestamp, const float* data) {
      if (!data)
        return;
      motionType_ = motionType;
      frame_number_ = frame_number;
      timestamp_ = timestamp;
      for (int i = 0; i < size_; i++) {
        data_[i] = data[i];
      }
    }

    void SetData(const rs::core::motion_sample& sample) {
      motionType_ = rs::utils::convert_motion_type(sample.type);
      frame_number_ = sample.frame_number;
      timestamp_ = sample.timestamp;
      for (int i = 0; i < size_; i++) {
        data_[i] = sample.data[i];
      }
    }

    MotionSample* ToMotionSample() {
      if (data_ == nullptr) return nullptr;

      auto motionData = new MotionSample();
      motionData->set_timestamp(timestamp_);
      motionData->set_frame_number(frame_number_);
      motionData->SetData(data_, size_);
      return motionData;
    }

   private:
    rs::event motionType_;
    uint64_t frame_number_;
    double timestamp_;
    float* data_;
    int32_t size_;
  };

  FrameDataAssembler() : fisheye_(nullptr), color_(nullptr), depth_(nullptr),
                      gyro_(nullptr), accel_(nullptr) {}
  ~FrameDataAssembler();

  void SetData(rs::stream stream, int width, int height,
      rs::format format, double timestamp, uint64_t frame_number,
      const char* data);
  void SetData(rs::event motionType, double timestamp,
               uint64_t frame_number, const float* data);

  void SetData(const rs::core::motion_sample& sample);

  void SetData(rs::core::image_interface* image);

  FrameData* ToFrameData();

 private:
  static std::string GetFormatString(rs::format format);
  StreamFrameData* fisheye_;
  StreamFrameData* color_;
  StreamFrameData* depth_;
  MotionData* gyro_;
  MotionData* accel_;
};

#endif  // _COMMON_FRAME_DATA_ASSEMBLER_H
