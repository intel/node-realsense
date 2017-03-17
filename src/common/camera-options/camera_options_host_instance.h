// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_OPTIONS_HOST_INSTANCE_H_
#define _CAMERA_OPTIONS_HOST_INSTANCE_H_

#include "camera_options_host.h"

class CameraOptionsHostInstance {
  CameraOptionsHostInstance();
  ~CameraOptionsHostInstance();

 public:
  static CameraOptionsHost* GetInstance();
};

#endif  // _CAMERA_OPTIONS_HOST_INSTANCE_H_
