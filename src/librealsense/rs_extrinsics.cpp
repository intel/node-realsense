// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "rs_extrinsics.h"

RSExtrinsics::RSExtrinsics(rs::extrinsics extrinsics)
    : Extrinsics(extrinsics.rotation, extrinsics.translation),
      extrinsics_(extrinsics),
      isIdentity_(extrinsics.is_identity()) {
  SetupTypedArrayHelper(&transform_, transform_store_, TRANSLATION_SIZE);
}

Float32ArrayHelper RSExtrinsics::transform(
    const Float32ArrayHelper& point) {
  DeepCopyFromTypedArrayHelper(point, transform_store_, TRANSLATION_SIZE);
  rs::float3 transform;
  transform.x = transform_store_[0];
  transform.y = transform_store_[1];
  transform.z = transform_store_[2];

  transform = extrinsics_.transform(transform);

  transform_store_[0] = transform.x;
  transform_store_[1] = transform.y;
  transform_store_[2] = transform.z;

  return transform_;
}

  RSExtrinsics& RSExtrinsics::operator = (const RSExtrinsics& rhs) {
    if (this != &rhs) {
      extrinsics_ = rhs.extrinsics_;
      isIdentity_ = rhs.isIdentity_;

      for (uint i = 0; i < TRANSLATION_SIZE; i++) {
        transform_store_[i] = rhs.transform_store_[i];
      }
    }
    return *this;
  }
