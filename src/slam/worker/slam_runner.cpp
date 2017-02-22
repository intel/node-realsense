// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <string>

#include "argument_wrapper.h"
#include "slam_runner.h"
#include "gen/nan__event_info.h"
#include "instance.h"
#include "tracking_result.h"

// Static function.
void SlamRunner::AsyncMessageCallback(uv_async_t* handle) {
  Nan::HandleScope scope;

  auto message = static_cast<AsyncMessage*>(handle->data);
  switch (message->type()) {
    case AsyncMessageType::kRequestDone: {
      auto req = static_cast<AsyncRequest*>(message->data());
      if (!req) break;

      req->Resolve();
      delete req;
      break;
    }
    case AsyncMessageType::kProcessFrameDone: {
      if (!message->channel()) {
        utils::OnSlamError("Bad channel for kProcessFrameDone message.");
        break;
      }
      if (!message->channel()->tracking_event_enabled()) break;

      auto trackedResult =
          static_cast<TrackedResultInternal*>(message->data());
      // Emit event.
      v8::Local<v8::Value> argv[2] = {
        Nan::New("tracking").ToLocalChecked(),
        NanEventInfo::NewInstance(trackedResult->ToEventInfo())};

      message->channel()->emitEvent(2, argv);
      break;
    }
    default:
      utils::OnSlamError(std::string("Unsupported message type - ") +
          std::to_string(static_cast<int>(message->type())));
  }
}
// static function.
void SlamRunner::AsyncCloseCallback(uv_handle_t* handle) {
  delete handle;
}

SlamRunner::SlamRunner() {
  flag_init_ = false;
  tracking_event_enabled_ = false;
  error_event_enabled_ = false;
  js_this_ = nullptr;
  async_ = nullptr;
  current_channel_.reset();
  worker_ = std::make_shared<SlamWorker>();
  slam_module_ = std::make_shared<SlamModule>();
}

SlamRunner::~SlamRunner() {
  slam_module_.reset();
  worker_.reset();
  js_this_ = nullptr;
  current_channel_.reset();
}

SlamRunner& SlamRunner::operator=(const SlamRunner& rhs) {
  if (&rhs != this) {
    this->async_ = rhs.async_;
    this->js_this_ = rhs.js_this_;
    this->worker_ = rhs.worker_;
    this->slam_module_ = rhs.slam_module_;
  }
  return *this;
}

bool SlamRunner::Init() {
  async_ = new uv_async_t;

  // Init channel to workers.
  current_channel_ = std::make_shared<AsyncChannel>(async_,
      std::make_shared<EventEmitter>(js_this_),
      tracking_event_enabled_, error_event_enabled_);
  worker_->set_channel(current_channel_);
  slam_module_->set_channel(current_channel_);

  // Init the async message callback.
  if (0 != uv_async_init(uv_default_loop(), async_, AsyncMessageCallback)) {
    return false;
  }
  flag_init_ = true;
  return worker_->Start();
}

void SlamRunner::StopWorker() {
  worker_->Stop();

  if (flag_init_ &&
      uv_is_active(reinterpret_cast<uv_handle_t*>(async_)) &&
      !uv_is_closing(reinterpret_cast<uv_handle_t*>(async_))) {
    uv_close(reinterpret_cast<uv_handle_t*>(async_), AsyncCloseCallback);
  }
  flag_init_ = false;
}
// ----------------move abrove to header file-------------------------
v8::Handle<v8::Promise> SlamRunner::createInstance(Instance* instance) {
  if (!Init()) {
    PromiseHelper pHelper;
    pHelper.RejectPromise(std::string("Failed to init the SLAM runner."));
    return pHelper.CreatePromise();
  }
  auto req = new ReqCreateInstance(SharedPtr(), instance);

  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::getInstanceOptions() {
  auto req = new ReqGetInstanceOptions(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::setInstanceOptions(
    const InstanceOptions& config) {
  auto req = new ReqSetInstanceOptions(SharedPtr(),
      new DictionaryInstanceOptions(config));
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapUpdate() {
  auto req = new ReqGetOccupancyMapUpdate(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapAsRgba(
    bool drawPoseTrajectory, bool drawOccupancyMap) {
  auto req = new ReqGetOccupancyMapAsRgba(SharedPtr(),
      new ArgumentUnit(drawPoseTrajectory, drawOccupancyMap));
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::getOccupancyMapBounds() {
  auto req = new ReqGetOccupancyMapBounds(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::saveOccupancyMapAsPpm(
    std::string file_name, bool draw_camera_trajectory) {
  auto req = new ReqSaveOccupancyMapAsPpm(SharedPtr(),
      new ArgumentWrapper(file_name, draw_camera_trajectory));
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::saveOccupancyMap(std::string fileName) {
  auto req = new ReqSaveOccupancyMap(SharedPtr(), new std::string(fileName));
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::start() {
  if (!flag_init_ && !Init()) {
    PromiseHelper pHelper;
    pHelper.RejectPromise(std::string("Failed to init the SLAM runner."));
    return pHelper.CreatePromise();
  }
  auto req = new ReqStart(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::getTrackingResult() {
  PromiseHelper promise_helper;
  if (!flag_init_) {
    promise_helper.RejectPromise(std::string(
        "Wrong state to getTrackingResult, start SLAM module first."));
  }
  auto req = new ReqGetTrackingResult(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}

v8::Handle<v8::Promise> SlamRunner::stop() {
  auto req = new ReqStop(SharedPtr());
  worker_->PostRequest(req);
  return req->GetReturnPromise();
}
