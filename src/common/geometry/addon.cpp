// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gen/nan__point2d.h"
#include "gen/nan__point3d.h"
#include "gen/nan__range.h"
#include "gen/nan__size2d.h"
#include "gen/nan__rect2d.h"

void initModule(v8::Local<v8::Object> exports) {
  NanPoint2D::Init(exports);

  NanPoint3D::Init(exports);

  NanRange::Init(exports);

  NanSize2D::Init(exports);

  NanRect2D::Init(exports);
}

NODE_MODULE(testAttributes, initModule);
