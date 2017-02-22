// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_OPTIONS_DEFAULT_H_
#define _CAMERA_OPTIONS_DEFAULT_H_

#include "camera_options_type.h"

//
// I have to admit this is poor naming... but at least it's straightforward
// Let me know good replacements, thanks. -- Kenny
//
class CameraOptionsDefault {
  CameraOptionsDefault(const CameraOptionsDefault&) = delete;
  CameraOptionsDefault& operator = (const CameraOptionsDefault&) = delete;

 public:
  CameraOptionsDefault() {}

  // Actually nobody will delete a pointer using this interface
  virtual ~CameraOptionsDefault() {}

  // Restore default
  virtual void ResetDefault(CameraOptionsType* target) const = 0;
};

#endif  // _CAMERA_OPTIONS_DEFAULT_H_
