// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gen/nan__context.h"
#include "gen/nan__device.h"
#include "gen/nan__frame_data.h"
#include "gen/nan__rs_extrinsics.h"

void initModule(v8::Local<v8::Object> exports) {
  NanContext::Init(exports);
  NanDevice::Init(exports);
  NanFrameData::Init(exports);
  NanRSExtrinsics::Init(exports);
}

NODE_MODULE(node_librealsense, initModule);
