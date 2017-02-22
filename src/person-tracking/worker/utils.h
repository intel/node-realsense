// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_UTILS_H
#define _WORKER_UTILS_H

#include <iostream>

#ifdef DEBUG
extern void DebugInfoMessage();
extern void DebugErrorMessage();
template<typename T, typename... Rest>
void DebugInfoMessage(T value, Rest... rest) {
  std::cout << value;
  DebugInfoMessage(rest...);
}

template<typename T, typename... Rest>
void DebugErrorMessage(T value, Rest... rest) {
  std::cerr << value;
  DebugErrorMessage(rest...);
}

#define DEBUG_INFO(...) \
std::cout << "Info: ";\
DebugInfoMessage(__VA_ARGS__);

#define DEBUG_ERROR(...) \
std::cerr << "Error: ";\
DebugErrorMessage(__VA_ARGS__);

#else
#define DEBUG_INFO(...)
#define DEBUG_ERROR(...)
#endif

#endif
