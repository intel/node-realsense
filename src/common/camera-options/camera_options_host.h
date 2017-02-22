// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_OPTIONS_HOST_H_
#define _CAMERA_OPTIONS_HOST_H_

#include <rs/core/video_module_interface.h>

#include <string>

#include "camera_options_io.h"
#include "camera_options_default.h"

#include "gen/camera_options.h"

class CameraOptionsHost {
  CameraOptionsHost(const CameraOptionsHost&) = delete;
  CameraOptionsHost& operator = (const CameraOptionsHost&) = delete;

 public:
  CameraOptionsHost();
  ~CameraOptionsHost();

  const CameraOptionsType& GetCameraOptions() const;
  CameraOptionsType& GetCameraOptions();
  void SetCameraOptions(const CameraOptionsType&);
  void PartiallyOverwriteCameraOptions(const CameraOptionsType&);

  // The underlying CameraOptionsConsumer obj or composite
  void SetCameraOptionsIO(CameraOptionsIOComposite*);
  CameraOptionsIOComposite* GetCameraOptionsIO() const;

  // The underlying CameraOptionsDefault obj or composite
  void SetCameraOptionsDefault(CameraOptionsDefault*);
  CameraOptionsDefault* GetCameraOptionsDefault() const;

  // Reset to default options
  void ResetDefaultCameraOptions();

  bool Validate() const;
  std::string GetValidationFailureMessage() const;

  // Read data from camera and fill in some of the members
  void ReadFromCameraData();

  // Apply data of all member to camera, after Validate() returns true
  void WriteToCamera();

 private:
  class CameraOptionsHostD* d_;
};

#endif  // _CAMERA_OPTIONS_HOST_H_
