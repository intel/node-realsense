// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "gen/nan__extrinsics.h"
#include "gen/nan__frame_data.h"
#include "gen/nan__localization_info.h"
#include "gen/nan__tracking_info.h"
#include "gen/nan__image.h"
#include "gen/nan__object_recognizer.h"
#include "gen/nan__intrinsics.h"
#include "gen/nan__motion_device_intrinsic.h"
#include "gen/nan__motion_sample.h"
#include "gen/nan__point2d.h"
#include "gen/nan__point3d.h"
#include "gen/nan__range.h"
#include "gen/nan__rect2d.h"
#include "gen/nan__recognition_info.h"
#include "gen/nan__size2d.h"
#include "gen/promise-helper.h"
#include "instance_creator.h"

void initModule(v8::Local<v8::Object> exports) {
  NanExtrinsics::Init(exports);
  NanFrameData::Init(exports);
  NanImage::Init(exports);
  NanObjectRecognizer::Init(exports);
  NanIntrinsics::Init(exports);
  NanLocalizationInfo::Init(exports);
  NanTrackingInfo::Init(exports);
  NanMotionDeviceIntrinsic::Init(exports);
  NanMotionSample::Init(exports);
  NanPoint2D::Init(exports);
  NanPoint3D::Init(exports);
  NanRange::Init(exports);
  NanRecognitionInfo::Init(exports);
  NanRect2D::Init(exports);
  NanSize2D::Init(exports);

  Nan::Export(exports,
              "createObjectRecognizer",
              CreateObjectRecognizerInstance);
}

NODE_MODULE(realsense_or, initModule);
