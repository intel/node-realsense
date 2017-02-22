// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CONTEXT_RUNNER_H_
#define _CONTEXT_RUNNER_H_

#include <memory>

namespace rs {
class context;
class device;
}  // namespace rs

class ContextRunner {
 public:
  ContextRunner();
  ~ContextRunner();

  int GetDeviceCount();
  rs::device* GetRSDevice(int index);

 protected:
  rs::context* GetRSContext();

 private:
  std::unique_ptr<rs::context> context_;
};

#endif  // _CONTEXT_RUNNER_H_
