// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "instance.h"

#include "gen/promise-helper.h"

Instance::Instance() {
}

Instance& Instance::operator=(const Instance& rhs) {
  if (&rhs != this) {
    this->js_this_.Reset(Nan::New(rhs.js_this_));
  }
  return *this;
}

Instance::~Instance() {
  SlamRunner::DestroySlamRunner();
  js_this_.Reset();
}

v8::Handle<v8::Promise> Instance::getCameraOptions() {
  return SlamRunner::GetSlamRunner()->getCameraOptions();
}

v8::Handle<v8::Promise> Instance::getInstanceOptions() {
  return SlamRunner::GetSlamRunner()->getInstanceOptions();
}

v8::Handle<v8::Promise> Instance::setCameraOptions(
    const CameraOptions& options) {
  std::string error;
  if (!options.CheckType(&error)) {
    PromiseHelper promise_helper;
    auto promise = promise_helper.CreatePromise();
    promise_helper.RejectPromise(error);
    return promise;
  }
  return SlamRunner::GetSlamRunner()->setCameraOptions(options);
}

v8::Handle<v8::Promise> Instance::setInstanceOptions(
    const InstanceOptions& options) {
  std::string error;
  if (!options.CheckType(&error)) {
    PromiseHelper promise_helper;
    auto promise = promise_helper.CreatePromise();
    promise_helper.RejectPromise(error);
    return promise;
  }
  return SlamRunner::GetSlamRunner()->setInstanceOptions(options);
}

v8::Handle<v8::Promise> Instance::setInstanceOptions() {
  PromiseHelper promise_helper;
  auto promise = promise_helper.CreatePromise();

  promise_helper.ResolvePromise();
  return promise;
}

v8::Handle<v8::Promise> Instance::start() {
  return SlamRunner::GetSlamRunner()->start();
}

v8::Handle<v8::Promise> Instance::stop() {
  return SlamRunner::GetSlamRunner()->stop();
}

v8::Handle<v8::Promise> Instance::pause() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::resume() {
  // TODO(widl-nan): fill your code here
}

v8::Handle<v8::Promise> Instance::reset() {
  return SlamRunner::GetSlamRunner()->reset();
}

v8::Handle<v8::Promise> Instance::restartTracking() {
  return SlamRunner::GetSlamRunner()->restartTracking();
}

v8::Handle<v8::Promise> Instance::getTrackingResult() {
  return SlamRunner::GetSlamRunner()->getTrackingResult();
}

v8::Handle<v8::Promise> Instance::getOccupancyMap() {
  return SlamRunner::GetSlamRunner()->getOccupancyMap(nullptr);
}

v8::Handle<v8::Promise> Instance::getOccupancyMap(
    const RegionOfInterest& roi) {
  std::string error;
  if (!roi.CheckType(&error)) {
    PromiseHelper promise_helper;
    auto promise = promise_helper.CreatePromise();
    promise_helper.RejectPromise(error);
    return promise;
  }
  return SlamRunner::GetSlamRunner()->getOccupancyMap(&roi);
}

v8::Handle<v8::Promise> Instance::getOccupancyMapAsRgba(
    const bool& drawPoseTrajectory, const bool& drawOccupancyMap) {
  return SlamRunner::GetSlamRunner()->getOccupancyMapAsRgba(
      drawPoseTrajectory, drawOccupancyMap);
}

v8::Handle<v8::Promise> Instance::getOccupancyMapUpdate(
    const RegionOfInterest& roi) {
  std::string error;
  if (!roi.CheckType(&error)) {
    PromiseHelper promise_helper;
    auto promise = promise_helper.CreatePromise();

    promise_helper.RejectPromise(error);
    return promise;
  }
  return SlamRunner::GetSlamRunner()->getOccupancyMapUpdate(&roi);
}

v8::Handle<v8::Promise> Instance::getOccupancyMapUpdate() {
  return SlamRunner::GetSlamRunner()->getOccupancyMapUpdate(nullptr);
}

v8::Handle<v8::Promise> Instance::getOccupancyMapBounds() {
  return SlamRunner::GetSlamRunner()->getOccupancyMapBounds();
}

v8::Handle<v8::Promise> Instance::loadOccupancyMap(
    const std::string& mapFileName) {
  return SlamRunner::GetSlamRunner()->loadOccupancyMap(mapFileName);
}

v8::Handle<v8::Promise> Instance::saveOccupancyMap(
    const std::string& mapFileName) {
  return SlamRunner::GetSlamRunner()->saveOccupancyMap(mapFileName);
}

v8::Handle<v8::Promise> Instance::saveOccupancyMapAsPpm(
    const std::string& mapFileName, const bool& drawCameraTrajectory) {
  return SlamRunner::GetSlamRunner()->saveOccupancyMapAsPpm(
      mapFileName, drawCameraTrajectory);
}

v8::Handle<v8::Promise> Instance::loadRelocalizationMap(
    const std::string& mapFileName) {
  return SlamRunner::GetSlamRunner()->loadRelocalizationMap(mapFileName);
}

v8::Handle<v8::Promise> Instance::saveRelocalizationMap(
    const std::string& mapFileName) {
  return SlamRunner::GetSlamRunner()->saveRelocalizationMap(mapFileName);
}

v8::Handle<v8::Promise> Instance::getRelocalizationPose() {
  return SlamRunner::GetSlamRunner()->getRelocalizationPose();
}
