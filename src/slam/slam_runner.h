// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _SLAM_RUNNER_H_
#define _SLAM_RUNNER_H_

#include <memory>
#include <string>

#include "slam_module.h"

class SlamRunner {
 private:
  std::shared_ptr<SlamModule> slam_module_;
  Nan::Persistent<v8::Object>* js_this_;
  SlamRunner();

 public:
  SlamRunner& operator=(const SlamRunner& rhs) = delete;
  ~SlamRunner();

  static SlamRunner* GetSlamRunner();
  static void DestroySlamRunner();

  std::shared_ptr<SlamModule> slam_module() { return slam_module_;}
  std::string GetSlamState() {
    return utils::SlamStateToString(slam_module_->state());
  }
  bool ShouldPopEvent(SlamEvent event);
  void SetJavaScriptThis(Nan::Persistent<v8::Object>* js_this) {
    js_this_ = js_this;
  }

  Nan::Persistent<v8::Object>* js_this() {
    return js_this_;
  }

  // ------- JS interfaces implementation --------
  // ------- All the functions name will be JavaScript style ---
  v8::Handle<v8::Promise> getInstanceOptions();
  v8::Handle<v8::Promise> setInstanceOptions(const InstanceOptions& config);
  v8::Handle<v8::Promise> start();
  v8::Handle<v8::Promise> stop();
  v8::Handle<v8::Promise> reset();
  v8::Handle<v8::Promise> getTrackingResult();
  v8::Handle<v8::Promise> getOccupancyMap(const RegionOfInterest* roi);
  v8::Handle<v8::Promise> getOccupancyMapUpdate(const RegionOfInterest* roi);
  v8::Handle<v8::Promise> getCameraOptions();
  v8::Handle<v8::Promise> setCameraOptions(const CameraOptions& config);
  v8::Handle<v8::Promise> restartTracking();
  v8::Handle<v8::Promise> saveOccupancyMap(const std::string& file_name);
  v8::Handle<v8::Promise> loadOccupancyMap(const std::string& file_name);
  v8::Handle<v8::Promise> saveOccupancyMapAsPpm(
      const std::string& file_name, bool draw_camera_trajectory);
  v8::Handle<v8::Promise> getOccupancyMapAsRgba(
      bool draw_pose_trajectory, bool draw_occupancy_map);
  v8::Handle<v8::Promise> getOccupancyMapBounds();
  v8::Handle<v8::Promise> loadRelocalizationMap(const std::string& file_name);
  v8::Handle<v8::Promise> saveRelocalizationMap(const std::string& file_name);
};

#endif  // _SLAM_RUNNER_H_
