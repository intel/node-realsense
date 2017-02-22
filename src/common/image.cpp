// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/image.h"

Image::Image()
  : width_(0),
    height_(0),
    timestamp_(0),
    frame_number_(0),
    data_({0}),
    own_data_(true) {
  // TODO(widl-nan): init your members
}

Image::~Image() {
  if (data_.data && own_data_) {
    delete [] data_.data;
  }
}

Image& Image::operator = (const Image& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

void Image::Assign(Image* img) {
  if (img && img != this) {
    width_ = img->width_;
    height_ = img->height_;
    format_ = img->format_;
    timestamp_ = img->timestamp_;
    frame_number_ = img->frame_number_;
    data_ = img->data_;
    // TODO(Donna): this is a walkaround to avoid multiple free data memory.
    memset(img, 0, sizeof(Image));
  }
}
