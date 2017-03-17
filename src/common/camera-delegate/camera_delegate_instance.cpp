// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "camera_delegate_instance.h"
#include "../process-singleton/process_singleton.h"

struct CameraDelegateTraits {
  static constexpr const char* shm_name = "node-realsense-shm-camera-delegate:";
};

typedef ProcessSingleton<CameraDelegate,
    CameraDelegateTraits> camera_delegate_singleton_t;

static const int required_dummy =
    camera_delegate_singleton_t::SetupCleanupHooks();

class CameraDelegateInstanceD {
  friend class CameraDelegateInstance;

  static void DestroyInstance() {
    camera_delegate_singleton_t::Cleanup();
    delete instance_;
    instance_ = nullptr;
  }

  static CameraDelegate* GetInstance() {
    if (!instance_) {
      if (!camera_delegate_singleton_t::QueryExistence()) {
        // Create one instance per one single OS process
        try {
          camera_delegate_singleton_t::SetInstance(new CameraDelegate());
        } catch (const boost::interprocess::interprocess_exception& e) {
          // Just absort it and do nothing
        }
      }
      instance_ = camera_delegate_singleton_t::GetInstance();
    }

    return instance_;
  }

  static CameraDelegate* instance_;
};

CameraDelegate* CameraDelegateInstanceD::instance_ = nullptr;

CameraDelegate* CameraDelegateInstance::GetInstance() {
  return CameraDelegateInstanceD::GetInstance();
}
