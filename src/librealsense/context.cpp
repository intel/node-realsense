// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "context.h"

#include <librealsense/rs.hpp>

#include "common/task/async_task_runner_instance.h"
#include "context_runner.h"
#include "rs_task.h"

#define POST_CONTEXT_PROMISE_TASK(type, method, message, ...) \
auto payload = new RSPayload<type>(); \
return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask( \
    new RSPromiseTask<type>( \
        std::bind(&ContextRunner::method, \
                  context_runner_.get(), \
                  ##__VA_ARGS__)), \
    payload, \
    #message); \

Context::Context() {
  context_runner_.reset(new ContextRunner());
}

Context::~Context() {
}

v8::Handle<v8::Promise> Context::getDeviceCount() {
  POST_CONTEXT_PROMISE_TASK(int, GetDeviceCount, {{GET_DEVICE_COUNT MESSAGE}});
}

v8::Handle<v8::Promise> Context::getDevice(const int32_t& index) {
  POST_CONTEXT_PROMISE_TASK(rs::device*, GetRSDevice, {{GET_RS_DEVICE MESSAGE}},
      index);
}
