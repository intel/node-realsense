// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_TRACKING_RESULT_INTERNAL_H_
#define _WORKER_PERSON_TRACKING_RESULT_INTERNAL_H_

#include <memory>
#include <vector>

#include "common/frame_data_assembler.h"
#include "person_tracking_result.h"
#include "worker/person_data_internal.h"

class PersonTrackResultInternal {
 public:
  PersonTrackResultInternal();
  ~PersonTrackResultInternal();

  PersonTrackingResult* ToPersonTrackResult();
  void Reset();

  // Transfer the owner ship of the framedata to the caller.
  // After this is called, the frame_data_ is reseted.
  FrameData* GrabFrameData();
 private:
  std::vector<std::shared_ptr<PersonDataInternal>> persons_;
  std::shared_ptr<FrameDataAssembler> frame_data_;
  friend class PersonTrackerAdapter;
};

#endif  // _WORKER_PERSON_TRACKING_RESULT_INTERNAL_H_
