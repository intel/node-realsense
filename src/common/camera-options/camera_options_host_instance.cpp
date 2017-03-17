// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "camera_options_host_instance.h"
#include "../process-singleton/process_singleton.h"

struct CameraOptionsHostTraits {
  static constexpr const char* shm_name = "node-realsense-shm-camera-options:";
};

typedef ProcessSingleton<CameraOptionsHost,
    CameraOptionsHostTraits> options_host_singleton_t;

static const int required_dummy =
    options_host_singleton_t::SetupCleanupHooks();

class CameraOptionsHostInstanceD {
  friend class CameraOptionsHostInstance;

  static void DestroyInstance() {
    options_host_singleton_t::Cleanup();
    delete instance_;
    instance_ = nullptr;
  }

  static CameraOptionsHost* GetInstance() {
    if (!instance_) {
      if (!options_host_singleton_t::QueryExistence()) {
        // Create one instance per one single OS process
        options_host_singleton_t::SetInstance(new CameraOptionsHost());
      }
      instance_ = options_host_singleton_t::GetInstance();
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
