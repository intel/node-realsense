// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "mask_info.h"

#include <memory>

MaskInfo::MaskInfo()
    : width_(0),
      height_(0),
      timestamp_(0),
      maskData_({0}) {
  // TODO(widl-nan): init your members
}

MaskInfo::MaskInfo(const MaskInfo& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in mask_info.h
}

MaskInfo::~MaskInfo() {
  if (maskData_.data)
     free(maskData_.data);
}

MaskInfo& MaskInfo::operator = (const MaskInfo& rhs) {
  if (&rhs != this) {
    width_ = rhs.width_;
    height_ = rhs.height_;
    maskData_.size = rhs.maskData_.size;
    maskData_.data = static_cast<char*>(malloc(maskData_.size));
    memcpy(maskData_.data, rhs.maskData_.data, maskData_.size);
  }

  return *this;
}

