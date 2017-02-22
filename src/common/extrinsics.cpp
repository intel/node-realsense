// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/extrinsics.h"

Extrinsics::Extrinsics() {
  ZerofillArray(rotation_store_, ROTATION_SIZE);
  ZerofillArray(translation_store_, TRANSLATION_SIZE);
  Setup();
}

Extrinsics::Extrinsics(const float* rotations, const float* translations) {
  Setup();
  SetRotation(rotations);
  SetTranslation(translations);
}

Extrinsics::Extrinsics(const Extrinsics& rhs) {
  CopyFrom(rhs);
}

Extrinsics::~Extrinsics() {
}

Extrinsics& Extrinsics::operator = (const Extrinsics& rhs) {
  CopyFrom(rhs);
  return *this;
}

void Extrinsics::Setup() {
  SetupTypedArrayHelper(&rotation_,
      rotation_store_,
      ROTATION_SIZE);
  SetupTypedArrayHelper(&translation_,
      translation_store_,
      TRANSLATION_SIZE);
}

void Extrinsics::CopyFrom(const Extrinsics& rhs) {
  if (this != &rhs) {
    Setup();
    DeepCopyFromTypedArrayHelper(rhs.rotation_,
        rotation_store_, ROTATION_SIZE);
    DeepCopyFromTypedArrayHelper(rhs.translation_,
        translation_store_, TRANSLATION_SIZE);
  }
}
