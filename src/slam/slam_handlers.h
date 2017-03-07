// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _SLAM_HANDLERS_H_
#define _SLAM_HANDLERS_H_

#include <rs_sdk.h>

#include <memory>

#include "async_channel.h"
#include "output_holder.h"

class SlamTrackingEventHandler : public rs::slam::tracking_event_handler {
 public:
  void on_restart() {
    // TODO(Donna): emit restart event to JS.
  }
};

class SlamEventHandler
    : public rs::core::video_module_interface::processing_event_handler {
 private:
  OutputHolder* output_holder_;
  std::mutex mutex_;

 public:
  SlamEventHandler() : output_holder_(nullptr) {}

  ~SlamEventHandler() {
    if (output_holder_ != nullptr) delete output_holder_;
  }

  OutputHolder* MoveResult();
  void module_output_ready(rs::core::video_module_interface* sender,
      rs::core::correlated_sample_set* sample);
};

#endif  // _SLAM_HANDLERS_H_
