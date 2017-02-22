// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_ALL_REQUESTS_H_
#define _WORKER_ALL_REQUESTS_H_

#include <memory>

#include "worker/async_request.h"

//
// Request for: CreateInstance
//
class ReqCreateInstance: public AsyncRequest {
 public:
  ReqCreateInstance(std::shared_ptr<SlamRunner> runner, void* argument)
      : AsyncRequest(runner, argument) {}
  explicit ReqCreateInstance(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: getInstanceOptions
//
class ReqGetInstanceOptions: public AsyncRequest {
 public:
  ReqGetInstanceOptions(std::shared_ptr<SlamRunner> runner, void* argument)
      : AsyncRequest(runner, argument) {}
  explicit ReqGetInstanceOptions(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: setInstanceOptions
//
class ReqSetInstanceOptions: public AsyncRequest {
 public:
  ReqSetInstanceOptions(std::shared_ptr<SlamRunner> runner, void* argument)
      : AsyncRequest(runner, argument) {}
  explicit ReqSetInstanceOptions(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: getTrackingResult
//
class ReqGetTrackingResult: public AsyncRequest {
 public:
  ReqGetTrackingResult(std::shared_ptr<SlamRunner> runner, void* argument)
      : AsyncRequest(runner, argument) {}
  explicit ReqGetTrackingResult(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: getOccupancyMapUpdate
//
class ReqGetOccupancyMapUpdate: public AsyncRequest {
 public:
  ReqGetOccupancyMapUpdate(std::shared_ptr<SlamRunner> runner, void* argument)
      :AsyncRequest(runner, argument) {}
  explicit ReqGetOccupancyMapUpdate(std::shared_ptr<SlamRunner> runner)
      :AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: getOccupancyMapAsRgba
//
class ReqGetOccupancyMapAsRgba: public AsyncRequest {
 public:
  ReqGetOccupancyMapAsRgba(std::shared_ptr<SlamRunner> runner, void* argument)
      :AsyncRequest(runner, argument) {}
  explicit ReqGetOccupancyMapAsRgba(std::shared_ptr<SlamRunner> runner)
      :AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: getOccupancyMapBounds
//
class ReqGetOccupancyMapBounds: public AsyncRequest {
 public:
  ReqGetOccupancyMapBounds(std::shared_ptr<SlamRunner> runner, void* argument)
      :AsyncRequest(runner, argument) {}
  explicit ReqGetOccupancyMapBounds(std::shared_ptr<SlamRunner> runner)
      :AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: saveOccpancyMapAsPpm
//
class ReqSaveOccupancyMapAsPpm: public AsyncRequest {
 public:
  ReqSaveOccupancyMapAsPpm(std::shared_ptr<SlamRunner> runner, void* argument)
      :AsyncRequest(runner, argument) {}
  explicit ReqSaveOccupancyMapAsPpm(std::shared_ptr<SlamRunner> runner)
      :AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: saveOccpancyMap
//
class ReqSaveOccupancyMap: public AsyncRequest {
 public:
  ReqSaveOccupancyMap(std::shared_ptr<SlamRunner> runner, void* argument)
      :AsyncRequest(runner, argument) {}
  explicit ReqSaveOccupancyMap(std::shared_ptr<SlamRunner> runner)
      :AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: start
//
class ReqStart: public AsyncRequest {
 public:
  explicit ReqStart(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

//
// Request for: start
//
class ReqStop: public AsyncRequest {
 public:
  explicit ReqStop(std::shared_ptr<SlamRunner> runner)
      : AsyncRequest(runner, nullptr) {}

  void DoWork() override;
  void Resolve() override;
};

#endif  // _WORKER_ALL_REQUESTS_H_
