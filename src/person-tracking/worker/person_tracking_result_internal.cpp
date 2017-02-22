// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "worker/person_tracking_result_internal.h"

#include <vector>

#include "gen/nan__person_info.h"

PersonTrackResultInternal::PersonTrackResultInternal() {
}

PersonTrackResultInternal::~PersonTrackResultInternal() {
  Reset();
}

FrameData* PersonTrackResultInternal::GrabFrameData() {
  if (frame_data_) {
    FrameData* ptr = frame_data_->ToFrameData();
    frame_data_.reset();
    return ptr;
  }
  return nullptr;
}

void PersonTrackResultInternal::Reset() {
  persons_.clear();
}

PersonTrackingResult* PersonTrackResultInternal::ToPersonTrackResult() {
  auto result = new PersonTrackingResult;
  std::vector<PersonInfo*> array;

  for ( unsigned int i = 0; i < persons_.size(); i++ ) {
    array.push_back(persons_[i]->ToPersonInfo());
  }

  ArrayHelper helper;
  helper.FromArrayOfImplT<NanPersonInfo>(array.begin(), array.end());
  result->persons_.FromArrayHelper(helper);
  return result;
}
