// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "instance.h"

Instance::Instance() {
  runner_ = std::make_shared<SlamRunner>();
}

Instance& Instance::operator=(const Instance& rhs) {
  if (&rhs != this) {
    this->runner_ = rhs.runner();
  }
  return *this;
}

Instance::~Instance() {
  runner_.reset();
  js_this_.Reset();
  if (REPLACE_ASYNC) SlamRunnerDev::DestroySlamRunner();
}

v8::Handle<v8::Promise> Instance::createInstance() {
  return runner_->createInstance(this);
}

v8::Handle<v8::Promise> Instance::getCameraOptions() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->getCameraOptions();
}

v8::Handle<v8::Promise> Instance::getInstanceOptions() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->getInstanceOptions();
  else
    return runner_->getInstanceOptions();
}

v8::Handle<v8::Promise> Instance::setCameraOptions(
    const CameraOptions& options) {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->setCameraOptions(options);
}

v8::Handle<v8::Promise> Instance::setInstanceOptions(
    const InstanceOptions& options) {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->setInstanceOptions(options);
  else
    return runner_->setInstanceOptions(options);
}

v8::Handle<v8::Promise> Instance::resetConfig() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::start() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->start();
  else
    return runner_->start();
}

v8::Handle<v8::Promise> Instance::stop() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->stop();
  else
    return runner_->stop();
}

v8::Handle<v8::Promise> Instance::pause() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::resume() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::reset() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::restartTracking() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::getTrackingResult() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->getTrackingResult();
  else
    return runner_->getTrackingResult();
}

v8::Handle<v8::Promise> Instance::getOccupancyMap(
    const RegionOfInterest& roi) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::getOccupancyMapAsRgba(
    const bool& drawPoseTrajectory, const bool& drawOccupancyMap) {
  return runner_->getOccupancyMapAsRgba(drawPoseTrajectory, drawOccupancyMap);
}

v8::Handle<v8::Promise> Instance::getOccupancyMapUpdate(
    const RegionOfInterest& roi) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::getOccupancyMapUpdate() {
  if (REPLACE_ASYNC)
    return SlamRunnerDev::GetSlamRunner()->getOccupancyMapUpdate();
  else
    return runner_->getOccupancyMapUpdate();
}

v8::Handle<v8::Promise> Instance::getOccupancyMapBounds() {
  return runner_->getOccupancyMapBounds();
}

v8::Handle<v8::Promise> Instance::loadOccupancyMap(
    const std::string& mapFileName) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::saveOccupancyMap(
    const std::string& mapFileName) {
  return runner_->saveOccupancyMap(mapFileName);
}

v8::Handle<v8::Promise> Instance::saveOccupancyMapAsPpm(
    const std::string& mapFileName, const bool& drawCameraTrajectory) {
  return runner_->saveOccupancyMapAsPpm(mapFileName, drawCameraTrajectory);
}

v8::Handle<v8::Promise> Instance::loadRelocalizationMap(
    const std::string& mapFileName) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::saveRelocalizationMap(
    const std::string& mapFileName) {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::getRelocalizationPose() {
  // TODO(widl-nan): fill your code here
}
