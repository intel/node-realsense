// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PROCESS_SINGLETON_H_
#define _PROCESS_SINGLETON_H_

#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <sstream>

using namespace boost::interprocess; // NOLINT(*)

template <class T, class TraitsT>
class ProcessSingleton {
  ProcessSingleton() = delete;
  ~ProcessSingleton() = delete;

 public:
  static bool QueryExistence() {
    try {
      shared_memory_object shared_mem(open_only,
          GetName().c_str(),
          read_write);
      return true;
    } catch (const boost::interprocess::interprocess_exception& e) {
    }
    return false;
  }

  static void SetInstance(T* ptr) {
    if (!hooks_set_) {
      // Force client to set hooks
      throw "Invoke SetupCleanupHooks() first";
    }

    shared_memory_object shared_mem(create_only,
        GetName().c_str(),
        read_write);
    shared_mem.truncate(shared_mem_size_);
    mapped_region region(shared_mem, read_write, 0, shared_mem_size_);
    void* shm = region.get_address();

    memcpy(shm, &ptr, sizeof(ptr));
  }

  static T* GetInstance() {
    shared_memory_object shared_mem(open_only,
        GetName().c_str(),
        read_write);
    shared_mem.truncate(shared_mem_size_);
    mapped_region region(shared_mem, read_write, 0, shared_mem_size_);
    void* shm = region.get_address();

    void* result = nullptr;
    memcpy(&result, shm, sizeof(result));
    return static_cast<T*>(result);
  }

  static void Cleanup() {
    shared_memory_object::remove(GetName().c_str());
  }

  static int SetupCleanupHooks() {
    hooks_set_ = true;

    static std::terminate_handler prev_t {std::set_terminate([]() {
      Cleanup();
      if (prev_t) {
        prev_t();
      }
      std::abort();
    })};

    atexit([]() {
      Cleanup();
    });

    std::set_unexpected([](){
      Cleanup();
      std::terminate();
    });

    return 1;
  }

  static void TestException() {
    throw "this goes to coredump";
  }

 private:
  static std::string GetName() {
    std::stringstream ss;
    ss << TraitsT::shm_name << getpid();
    return ss.str();
  }

  static const size_t shared_mem_size_ = sizeof (T*);
  static bool hooks_set_;
};

template <class T, class TraitsT>
bool ProcessSingleton<T, TraitsT>::hooks_set_ = false;


#endif  // _PROCESS_SINGLETON_H_
