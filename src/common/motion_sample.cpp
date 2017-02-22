// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/motion_sample.h"

MotionSample::MotionSample() {
  ZerofillArray(data_store_, DATA_SIZE);
  Setup();
}

MotionSample::MotionSample(const MotionSample& rhs) {
  CopyFrom(rhs);
}

MotionSample::~MotionSample() {
}

MotionSample& MotionSample::operator = (const MotionSample& rhs) {
  CopyFrom(rhs);
  return *this;
}

void MotionSample::Setup() {
  SetupTypedArrayHelper(&data_,
      data_store_,
      DATA_SIZE);
}

void MotionSample::CopyFrom(const MotionSample& rhs) {
  if (this != &rhs) {
    Setup();
    DeepCopyFromTypedArrayHelper(rhs.data_,
        data_store_, DATA_SIZE);
  }
}
