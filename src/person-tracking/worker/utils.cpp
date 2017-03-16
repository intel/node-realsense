// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/utils.h"

#include <iostream>

void DebugInfoMessage() {
  std::cout << std::endl << std::flush;
}

void DebugErrorMessage() {
  std::cerr << std::endl << std::flush;
}
