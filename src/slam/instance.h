// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _INSTANCE_H_
#define _INSTANCE_H_

#include <node.h>
#include <v8.h>

#include <memory>
#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "gen/camera_options.h"
#include "gen/instance_options.h"
#include "gen/region_of_interest.h"
#include "slam_runner.h"

class Instance {
 public:
  Instance();

  Instance(const Instance& rhs) = delete;

  ~Instance();

  Instance& operator=(const Instance& rhs);

 public:
  std::string get_state() const {
    return SlamRunner::GetSlamRunner()->GetSlamState();
  }

  v8::Handle<v8::Promise> getCameraOptions();

  v8::Handle<v8::Promise> getInstanceOptions();

  v8::Handle<v8::Promise> setCameraOptions(const CameraOptions& options);

  v8::Handle<v8::Promise> setInstanceOptions();

  v8::Handle<v8::Promise> setInstanceOptions(const InstanceOptions& options);

  v8::Handle<v8::Promise> start();

  v8::Handle<v8::Promise> stop();

  v8::Handle<v8::Promise> pause();

  v8::Handle<v8::Promise> resume();

  v8::Handle<v8::Promise> reset();

  v8::Handle<v8::Promise> restartTracking();

  v8::Handle<v8::Promise> getTrackingResult();

  v8::Handle<v8::Promise> getOccupancyMap();

  v8::Handle<v8::Promise> getOccupancyMap(const RegionOfInterest& roi);

  v8::Handle<v8::Promise> getOccupancyMapAsRgba(
      const bool& drawPoseTrajectory, const bool& drawOccupancyMap);

  v8::Handle<v8::Promise> getOccupancyMapUpdate();

  v8::Handle<v8::Promise> getOccupancyMapUpdate(const RegionOfInterest& roi);

  v8::Handle<v8::Promise> getOccupancyMapBounds();

  v8::Handle<v8::Promise> loadOccupancyMap(const std::string& mapFileName);

  v8::Handle<v8::Promise> saveOccupancyMap(const std::string& mapFileName);

  v8::Handle<v8::Promise> saveOccupancyMapAsPpm(
      const std::string& mapFileName, const bool& drawCameraTrajectory);

  v8::Handle<v8::Promise> loadRelocalizationMap(const std::string& mapFileName);

  v8::Handle<v8::Promise> saveRelocalizationMap(const std::string& mapFileName);

  v8::Handle<v8::Promise> getRelocalizationPose();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    js_this_.Reset(obj);
    SlamRunner::GetSlamRunner()->SetJavaScriptThis(&js_this_);
  }

 private:
  Nan::Persistent<v8::Object> js_this_;
};

#endif  // _INSTANCE_H_
