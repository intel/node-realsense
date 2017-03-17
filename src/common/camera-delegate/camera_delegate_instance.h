// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_DELEGATE_INSTANCE_H_
#define _CAMERA_DELEGATE_INSTANCE_H_

#include "camera_delegate.h"

class CameraDelegateInstance {
  CameraDelegateInstance() = delete;
  ~CameraDelegateInstance() = delete;

 public:
  static CameraDelegate* GetInstance();
  static void DestroyInstance();
};

#endif  // _CAMERA_DELEGATE_INSTANCE_H_
