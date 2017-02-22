// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _IMAGE_UTILS_H_
#define _IMAGE_UTILS_H_

namespace image_utils {
inline int8_t GetPixelSize(rs::format format) {
  switch (format) {
    case rs::format::any:
      return 0;

    case rs::format::z16:
      return 2;

    case rs::format::disparity16:
      return 2;

    case rs::format::xyz32f:
      return 4;

    case rs::format::yuyv:
      return 2;

    case rs::format::rgb8:
      return 3;

    case rs::format::bgr8:
      return 3;

    case rs::format::rgba8:
      return 4;

    case rs::format::bgra8:
      return 4;

    case rs::format::y8:
      return 1;

    case rs::format::y16:
      return 2;

    case rs::format::raw8:
      return 1;

    case rs::format::raw10:
      return 0;  // not supported

    case rs::format::raw16:
      return 2;

    default:
      return 0;
  }
}
}  // namespace image_utils


#endif  // _IMAGE_UTILS_H_
