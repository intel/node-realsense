// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <string>

#include "camera_options_host.h"

class CameraOptionsHostD {
  friend class CameraOptionsHost;

  CameraOptionsHostD() {
    io_interface_ = new CameraOptionsIOComposite();
    default_resetter_ = nullptr;
    SetupCameraOptions();  // Temp solution for default value
  }

  ~CameraOptionsHostD() {
    delete io_interface_;
  }

  void SetupCameraOptions() {
    camera_options_.has_member_color = true;
    auto& color_stream = camera_options_.member_color;
    color_stream.has_member_width = true;
    color_stream.has_member_height = true;
    color_stream.has_member_frameRate = true;
    color_stream.has_member_isEnabled = true;
    color_stream.member_isEnabled = true;
    color_stream.member_width = 640;
    color_stream.member_height = 480;
    color_stream.member_frameRate = 30;

    camera_options_.has_member_fisheye = true;
    auto& fisheye_stream = camera_options_.member_fisheye;
    fisheye_stream.has_member_width = true;
    fisheye_stream.has_member_height = true;
    fisheye_stream.has_member_frameRate = true;
    fisheye_stream.has_member_isEnabled = true;
    fisheye_stream.member_isEnabled = true;
    fisheye_stream.member_width = 640;
    fisheye_stream.member_height = 480;
    fisheye_stream.member_frameRate = 30;

    camera_options_.has_member_depth = true;
    auto& depth_stream = camera_options_.member_depth;
    depth_stream.has_member_width = true;
    depth_stream.has_member_height = true;
    depth_stream.has_member_frameRate = true;
    depth_stream.has_member_isEnabled = true;
    depth_stream.member_isEnabled = true;
    depth_stream.member_width = 320;
    depth_stream.member_height = 240;
    depth_stream.member_frameRate = 30;
  }

  CameraOptionsIOComposite* io_interface_;
  CameraOptionsType         camera_options_;
  CameraOptionsDefault*     default_resetter_;
};

CameraOptionsHost::CameraOptionsHost() {
  d_ = new CameraOptionsHostD();
}

CameraOptionsHost::~CameraOptionsHost() {
  delete d_;
}

void CameraOptionsHost::SetCameraOptionsIO(CameraOptionsIOComposite* target) {
  d_->io_interface_ = target;
}

CameraOptionsIOComposite* CameraOptionsHost::GetCameraOptionsIO() const {
  return d_->io_interface_;
}

void CameraOptionsHost::SetCameraOptionsDefault(CameraOptionsDefault* reset) {
  d_->default_resetter_ = reset;
}

CameraOptionsDefault* CameraOptionsHost::GetCameraOptionsDefault() const {
  return d_->default_resetter_;
}

void CameraOptionsHost::ResetDefaultCameraOptions() {
  d_->default_resetter_->ResetDefault(&d_->camera_options_);
}

bool CameraOptionsHost::Validate() const {
  return d_->io_interface_->IsSupported(&d_->camera_options_);
}

std::string CameraOptionsHost::GetValidationFailureMessage() const {
  return d_->io_interface_->GetUnsupportedMessage();
}

const CameraOptionsType& CameraOptionsHost::GetCameraOptions() const {
  return d_->camera_options_;
}

CameraOptionsType& CameraOptionsHost::GetCameraOptions() {
  return d_->camera_options_;
}

void CameraOptionsHost::SetCameraOptions(const CameraOptionsType& rhs) {
  d_->camera_options_ = rhs;
}

void CameraOptionsHost::PartiallyOverwriteCameraOptions(
    const CameraOptionsType& options) {
  d_->camera_options_.IncrementallyCopyFrom(options);
}

void CameraOptionsHost::ReadFromCameraData() {
  d_->io_interface_->PartiallyFillInCameraData(&d_->camera_options_);
}

void CameraOptionsHost::WriteToCamera() {
  d_->io_interface_->ApplyToCamera(&d_->camera_options_);
}
