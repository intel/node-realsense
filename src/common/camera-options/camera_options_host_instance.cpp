// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "camera_options_host_instance.h"

class CameraOptionsHostInstanceD {
  friend class CameraOptionsHostInstance;

  static void DestroyInstance() {
    delete instance_;
  }

  static CameraOptionsHost* GetInstance() {
    if (!instance_) {
      instance_ = new CameraOptionsHost();
    }

    return instance_;
  }

  static CameraOptionsHost* instance_;
};

CameraOptionsHost* CameraOptionsHostInstanceD::instance_ = nullptr;

CameraOptionsHostInstance::CameraOptionsHostInstance() {
}

CameraOptionsHostInstance::~CameraOptionsHostInstance() {
}

CameraOptionsHost* CameraOptionsHostInstance::GetInstance() {
  return CameraOptionsHostInstanceD::GetInstance();
}
