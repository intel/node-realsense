// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _RS_TASK_H_
#define _RS_TASK_H_

#include <librealsense/rs.hpp>
#include <string>

#include "common/task/async_task.h"
#include "extrinsics.h"
#include "frame_data.h"
#include "gen/mode.h"
#include "gen/nan__frame_data.h"
#include "rs_payload.h"

class RSExtrinsics;

class RSFrameReadyEventTask : public DirectEventEmitterTask {
 public:
  explicit RSFrameReadyEventTask(Nan::Persistent<v8::Object>* obj)
      : js_this_(obj) {
  }
  ~RSFrameReadyEventTask() override {}

  v8_object_t QueryJavaScriptThis() override {
    return Nan::New(*js_this_);
  }

  size_t GetEventCount() override {
    return 1;
  }

  bool ShouldPopEvent(size_t event_index) override;

  v8_value_t PopEventData(size_t event_index) override {
    return NanFrameData::NewInstance(GetPayload()->GetPayloadData());
  }

  RSPayload<FrameData*>* GetPayload() override {
    return reinterpret_cast<RSPayload<FrameData*>*>(
        AsyncTask::GetPayload());
  }

  std::string GetEventName(size_t event_index) const override {
    return "frameready";
  }

 private:
  Nan::Persistent<v8::Object>* js_this_;
};

template<typename T>
class RSPromiseTask : public PromiseTask {
 public:
  explicit RSPromiseTask(std::function<T()> task) : task_(task) {}
  ~RSPromiseTask() override {}

  void WorkerThreadExecute() override {
    try {
      GetPayload()->SetPayloadData(task_());
      task_state = Successful;
    } catch (const rs::error& err) {
      GetPayload()->SetErrorMsg(err.what());
      task_state = Failed;
    } catch (...) {
      GetPayload()->SetErrorMsg("Unknown error.");
      task_state = Failed;
    }
  }

  RSPayload<T>* GetPayload() override {
    return reinterpret_cast<RSPayload<T>*>(AsyncTask::GetPayload());
  }

  v8::Local<v8::Value> GetResolved() override {
    auto payload = GetPayload();
    return static_cast<v8::Local<v8::Value>>(
        Nan::New(payload->GetPayloadData()));
  }

  v8_value_t GetRejected() override {
    return Nan::New(GetPayload()->ErrorMsg()).ToLocalChecked();
  }

 private:
  std::function<T()> task_;
};

template<>
void RSPromiseTask<void>::WorkerThreadExecute();

template<>
v8::Local<v8::Value> RSPromiseTask<std::string>::GetResolved();

template<>
v8::Local<v8::Value> RSPromiseTask<DictionaryMode>::GetResolved();

template<>
v8::Local<v8::Value> RSPromiseTask<void>::GetResolved();

template<>
v8::Local<v8::Value> RSPromiseTask<rs::device*>::GetResolved();

template<>
v8::Local<v8::Value> RSPromiseTask<RSExtrinsics*>::GetResolved();
#endif  // _RS_TASK_H_
