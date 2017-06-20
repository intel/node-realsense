// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "rs_task.h"

#include "device.h"
#include "device_runner.h"
#include "gen/nan__device.h"
#include "gen/nan__rs_extrinsics.h"

bool RSFrameReadyEventTask::ShouldPopEvent(size_t event_index) {
  const char* api_name_string = "listenerCount";

  v8::Local<v8::String> api_name = Nan::New(api_name_string).ToLocalChecked();
  v8::Local<v8::Object> js_this = Nan::New(*js_this_);
  Nan::Maybe<bool> has_api_maybe = Nan::Has(js_this, api_name);

  bool has_api = has_api_maybe.FromMaybe(false);

  if (has_api) {
    v8::Local<v8::Value> args[1] =
        {Nan::New("frameready").ToLocalChecked()};
    v8::Local<v8::Value> result =
        Nan::MakeCallback(js_this, api_name_string, 1, args);
    int32_t count = result->Int32Value();

    if (count) {
      return true;
    }
  }
  return false;
}

template<>
void RSPromiseTask<void>::WorkerThreadExecute() {
  try {
    task_();
    task_state = Successful;
  } catch (const rs::error& err) {
    GetPayload()->SetErrorMsg(err.what());
    task_state = Failed;
  } catch (...) {
    GetPayload()->SetErrorMsg("Unknown error.");
    task_state = Failed;
  }
}

template<>
v8::Local<v8::Value> RSPromiseTask<std::string>::GetResolved() {
  auto payload = GetPayload();
  return static_cast<v8::Local<v8::Value>>(
      Nan::New(payload->GetPayloadData()).ToLocalChecked());
}

template<>
v8::Local<v8::Value> RSPromiseTask<DictionaryMode>::GetResolved() {
  Mode mode;
  GetPayload()->GetPayloadData().ExportTo(mode);
  return static_cast<v8::Local<v8::Object>>(mode);
}

template<>
v8::Local<v8::Value> RSPromiseTask<void>::GetResolved() {
  return PromiseTask::GetResolved();
}

template<>
v8::Local<v8::Value> RSPromiseTask<rs::device*>::GetResolved() {
  auto device = new Device();
  device->SetDeviceRunner(new DeviceRunner(GetPayload()->GetPayloadData()));
  return static_cast<v8::Local<v8::Object>>(
      NanDevice::NewInstance(device));
}

template<>
v8::Local<v8::Value> RSPromiseTask<RSExtrinsics*>::GetResolved() {
  return NanRSExtrinsics::NewInstance(GetPayload()->GetPayloadData());
}
