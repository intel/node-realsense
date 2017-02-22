// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _RS_PAYLOAD_H_
#define _RS_PAYLOAD_H_

#include <string>

#include "common/task/async_task.h"

class AsyncTaskPayloadWithErrorMsg : public AsyncTaskPayload {
 public:
  void SetErrorMsg(const std::string& err) { err_ = err; }
  std::string ErrorMsg() { return err_; }

 private:
  std::string err_;
};

template<typename T>
class RSPayload : public AsyncTaskPayloadWithErrorMsg {
 public:
  T GetPayloadData() { return data_; }
  void SetPayloadData(const T& data) {
    data_ = data;
  }

 private:
  T data_;
};

template<>
class RSPayload<void> : public AsyncTaskPayloadWithErrorMsg {
};

#endif  // _RS_PAYLOAD_H_
