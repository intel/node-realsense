// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_SLAM_RUNNER_H_
#define _WORKER_SLAM_RUNNER_H_

#include <memory>
#include <string>

#include "all_requests.h"
#include "slam_worker.h"
#include "slam_module.h"

class Instance;
class SlamRunner : public std::enable_shared_from_this<SlamRunner>{
 private:
  std::shared_ptr<SlamWorker> worker_;
  std::shared_ptr<SlamModule> slam_module_;
  Nan::Persistent<v8::Object>* js_this_;
  uv_async_t* async_;
  bool flag_init_;
  bool tracking_event_enabled_;
  bool error_event_enabled_;
  std::shared_ptr<AsyncChannel> current_channel_;

  static void AsyncMessageCallback(uv_async_t* handle);
  static void AsyncCloseCallback(uv_handle_t* handle);
  bool Init();

  std::shared_ptr<SlamRunner> SharedPtr() {
    return shared_from_this();
  }

 public:
  SlamRunner();
  SlamRunner& operator=(const SlamRunner& rhs);
  ~SlamRunner();

  void StopWorker();

  std::shared_ptr<SlamModule> slam_module() const {
    return slam_module_;
  }

  bool tracking_event_enabled() const {
    return this->tracking_event_enabled_;
  }

  void set_tracking_event_enabled(bool newValue) {
    this->tracking_event_enabled_ = newValue;
    if (!current_channel_) return;

    current_channel_->set_tracking_event_enabled(tracking_event_enabled_);
  }

  bool error_event_enabled() const {
    return this->error_event_enabled_;
  }

  void set_error_event_enabled(bool newValue) {
    this->error_event_enabled_ = newValue;
    if (!current_channel_) return;

    current_channel_->set_error_event_enabled(error_event_enabled_);
  }

  std::string GetSlamState() {
    switch (slam_module_->state()) {
      case SlamState::kReady: return "ready";
      case SlamState::kTracking: return "tracking";
      case SlamState::kPaused: return "paused";
                               // IDLE state should not be exposed to user.
      default: return "";
    }
  }

  void SetJavaScriptThis(Nan::Persistent<v8::Object>* js_this) {
    js_this_ = js_this;
    if (!current_channel_) return;

    current_channel_->set_emitter(std::make_shared<EventEmitter>(js_this_));
  }

  // ------- JS interfaces implementation --------
  // ------- All the functions name will be JavaScript style ---
  v8::Handle<v8::Promise> createInstance(Instance* instance);
  v8::Handle<v8::Promise> getInstanceOptions();
  v8::Handle<v8::Promise> setInstanceOptions(const InstanceOptions& config);
  v8::Handle<v8::Promise> getTrackingResult();
  v8::Handle<v8::Promise> getOccupancyMapUpdate();
  v8::Handle<v8::Promise> getOccupancyMapAsRgba(
      bool drawPoseTrajectory, bool drawOccupancyMap);
  v8::Handle<v8::Promise> getOccupancyMapBounds();
  v8::Handle<v8::Promise> saveOccupancyMap(std::string fileName);
  v8::Handle<v8::Promise> saveOccupancyMapAsPpm(
      std::string file_name, bool draw_camera_trajectory);
  v8::Handle<v8::Promise> start();
  v8::Handle<v8::Promise> stop();
};

#endif  // _WORKER_SLAM_RUNNER_H_
