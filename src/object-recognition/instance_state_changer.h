// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _INSTANCE_STATE_CHANGER_H_
#define _INSTANCE_STATE_CHANGER_H_

#include <string>

#define OR_STATE_READY   ("ready")
#define OR_STATE_RUNNING ("running")
#define OR_STATE_PAUSED  ("paused")
#define OR_STATE_ERRORED ("errored")

class InstanceStateChanger {
 public:
  InstanceStateChanger() {}
  virtual ~InstanceStateChanger() {}

  virtual std::string GetInstanceState() const = 0;
  virtual void SetInstanceState(const std::string& state) = 0;
};

#endif  // _INSTANCE_STATE_CHANGER_H_
