// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_FLOAT_EQUAL_H_
#define _COMMON_GEOMETRY_FLOAT_EQUAL_H_
#include <cmath>

inline bool double_equal(const double& v1, const double& v2) {
  return std::isgreaterequal(v1, v2) && std::isgreaterequal(v2, v1);
}

inline bool float_equal(const float& v1, const float& v2) {
  return std::isgreaterequal(v1, v2) && std::isgreaterequal(v2, v1);
}

#endif  // _COMMON_GEOMETRY_FLOAT_EQUAL_H_
