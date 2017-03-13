// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <string>

#include "slam_runner.h"

#include "slam_async_tasks.h"
#include "common/task/async_task_runner_instance.h"

static SlamRunner* g_slam_runner = nullptr;

SlamRunner* SlamRunner::GetSlamRunner() {
  if (!g_slam_runner)
    g_slam_runner = new SlamRunner();

  return g_slam_runner;
}

void SlamRunner::DestroySlamRunner() {
  delete g_slam_runner;
  g_slam_runner = nullptr;
}

SlamRunner::SlamRunner() {
  slam_module_ = std::make_shared<SlamModule>();
}

SlamRunner::~SlamRunner() {
  slam_module_.reset();
}
bool SlamRunner::ShouldPopEvent(SlamEvent event) {
  const char* api_name_string = "listenerCount";
  if (!js_this_) return false;

  v8::Local<v8::String> api_name = Nan::New(api_name_string).ToLocalChecked();
  v8::Local<v8::Object> js_this = Nan::New(*js_this_);
  Nan::Maybe<bool> has_api_maybe = Nan::Has(js_this, api_name);
  if (!has_api_maybe.FromMaybe(false)) return false;

  if (event == SlamEvent::kError || (event == SlamEvent::kTracking &&
      slam_module_->state() == SlamState::kTracking)) {
    v8::Local<v8::Value> args[1] =
    {Nan::New(utils::SlamEventToString(event)).ToLocalChecked()};
    v8::Local<v8::Value> result =
    Nan::MakeCallback(js_this, api_name_string, 1, args);
    int32_t count = result->Int32Value();
    return count > 0;
  }
  return false;
}
/////////////////////////////////////////////////////////////
// API implementations.
/////////////////////////////////////////////////////////////
v8::Handle<v8::Promise> SlamRunner::getInstanceOptions() {
  auto payload = new InstanceOptionsTaskPayload(this,
      new DictionaryInstanceOptions());

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetInstanceOptionsTask(),
      payload,
      "{{GET_INSTANCE_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> SlamRunner::setInstanceOptions(
    const InstanceOptions& options) {
  auto payload = new InstanceOptionsTaskPayload(this,
      new DictionaryInstanceOptions(options));

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SetInstanceOptionsTask(),
      payload,
      "{{SET_INSTANCE_OPTIONS MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::getCameraOptions() {
  auto payload = new CameraOptionsTaskPayload(this,
      new DictionaryCameraOptions());

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetCameraOptionsTask(),
      payload,
      "{{GET_CAMERA_OPTIONS MESSAGE}}");
}

v8::Local<v8::Promise> SlamRunner::setCameraOptions(
    const CameraOptions& options) {
  auto payload = new CameraOptionsTaskPayload(this,
      new DictionaryCameraOptions(options));

  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SetCameraOptionsTask(),
      payload,
      "{{SET_CAMERA_OPTIONS MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::start() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new StartTask(),
      new SlamPayload<void>(this),
      "{{START MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::stop() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new StopTask(),
      new SlamPayload<void>(this),
      "{{STOP MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::reset() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new ResetTask(),
      new SlamPayload<void>(this),
      "{{RESET MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::getTrackingResult() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetTrackingResultTask(),
      new TrackingResultPayload(this, nullptr),
      "{{GET_TRACKING_RESULT MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapUpdate() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetOccupancyMapUpdateTask(),
      new SlamPayload<OccupancyMapData*>(this, new OccupancyMapData()),
      "{{GET_OCCUPANCY_MAP_UPDATE MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::restartTracking() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new RestartTrackingTask(),
      new SlamPayload<void>(this),
      "{{RESTART_TRACKING MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::saveOccupancyMap(
    const std::string& file_name) {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SaveOccupancyMapTask(),
      new SlamPayload<std::string>(this, file_name),
      "{{SAVE_OCCUPANCY_MAP MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::loadOccupancyMap(
    const std::string& file_name) {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new LoadOccupancyMapTask(),
      new SlamPayload<std::string>(this, file_name),
      "{{LOAD_OCCUPANCY_MAP MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::saveOccupancyMapAsPpm(
    const std::string& file_name, bool draw_camera_trajectory) {
  auto wrapper = new ParameterWrapperForSavingPpmMap(
      file_name, draw_camera_trajectory);
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SaveOccupancyMapAsPpmTask(),
      new SavePpmMapPayload(this, wrapper),
      "{{SAVE_OCCUPANCY_MAP_AS_PPM MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapAsRgba(
    bool draw_pose_trajectory, bool draw_occupancy_map) {
  auto wrapper = new ParameterWrapperForGetRgbaMap(
      draw_pose_trajectory, draw_occupancy_map);
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetOccupancyMapAsRgbaTask(),
      new GetRgbaMapPayload(this, wrapper),
      "{{GET_OCCUPANCY_MAP_AS_RGBA MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapBounds() {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new GetOccupancyMapBoundsTask(),
      new SlamPayload<OccupancyMapBounds*>(this, nullptr),
      "{{GET_OCCUPANCY_MAP_BOUNDS MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::loadRelocalizationMap(
    const std::string& file_name) {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new LoadRelocalizationMapTask(),
      new SlamPayload<std::string>(this, file_name),
      "{{LOAD_RELOCALIZATION_MAP MESSAGE}}");
}

v8::Handle<v8::Promise> SlamRunner::saveRelocalizationMap(
    const std::string& file_name) {
  return AsyncTaskRunnerInstance::GetInstance()->PostPromiseTask(
      new SaveRelocalizationMapTask(),
      new SlamPayload<std::string>(this, file_name),
      "{{SAVE_RELOCALIZATION_MAP MESSAGE}}");
}
