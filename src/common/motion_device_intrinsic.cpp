// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/motion_device_intrinsic.h"

MotionDeviceIntrinsic::MotionDeviceIntrinsic() {
  ZerofillArray(data_store_, DATA_SIZE);
  ZerofillArray(noise_variances_store_, NOISE_VARIANCE_SIZE);
  ZerofillArray(bias_variances_store_, BIAS_VARIANCE_SIZE);
  Setup();
}

MotionDeviceIntrinsic::MotionDeviceIntrinsic(const float* data,
      const float* noise_variances, const float* bias_variances) {
  Setup();
  SetData(data);
  SetNoiseVariances(noise_variances);
  SetBiasVariances(bias_variances);
}

MotionDeviceIntrinsic::MotionDeviceIntrinsic(const MotionDeviceIntrinsic& rhs) {
  CopyFrom(rhs);
}

MotionDeviceIntrinsic::~MotionDeviceIntrinsic() {
}

MotionDeviceIntrinsic& MotionDeviceIntrinsic::operator = (
    const MotionDeviceIntrinsic& rhs) {
  if (&rhs != this) {
    CopyFrom(rhs);
  }
  return *this;
}

void MotionDeviceIntrinsic::Setup() {
  SetupTypedArrayHelper(&data_,
      data_store_,
      DATA_SIZE);

  SetupTypedArrayHelper(&noiseVariances_,
      noise_variances_store_,
      NOISE_VARIANCE_SIZE);

  SetupTypedArrayHelper(&biasVariances_,
      bias_variances_store_,
      BIAS_VARIANCE_SIZE);
}

void MotionDeviceIntrinsic::CopyFrom(const MotionDeviceIntrinsic& rhs) {
  if (this != &rhs) {
    Setup();
    DeepCopyFromTypedArrayHelper(rhs.data_,
        data_store_, DATA_SIZE);
    DeepCopyFromTypedArrayHelper(rhs.noiseVariances_,
        noise_variances_store_, NOISE_VARIANCE_SIZE);
    DeepCopyFromTypedArrayHelper(rhs.biasVariances_,
        bias_variances_store_, BIAS_VARIANCE_SIZE);
  }
}
