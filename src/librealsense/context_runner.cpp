// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "context_runner.h"

#include <librealsense/rs.hpp>

ContextRunner::ContextRunner()
  : context_(nullptr) {
}

ContextRunner::~ContextRunner() {
}

int ContextRunner::GetDeviceCount() {
  return GetRSContext()->get_device_count();
}

rs::device* ContextRunner::GetRSDevice(int index) {
  return GetRSContext()->get_device(index);
}

rs::context* ContextRunner::GetRSContext() {
  if (!context_) {
    context_.reset(new rs::context());
  }
  return context_.get();
}
