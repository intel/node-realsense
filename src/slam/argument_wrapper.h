// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _ARGUMENT_WRAPPER_H_
#define _ARGUMENT_WRAPPER_H_

#include <string>

class ArgumentWrapper {
 public:
  ArgumentWrapper(std::string file_name, bool draw_camera_trajectory) {
    this->file_name = file_name;
    this->draw_camera_trajectory = draw_camera_trajectory;
  }

 public:
  std::string file_name;
  bool draw_camera_trajectory;
};

#endif  // _ARGUMENT_WRAPPER_H_
