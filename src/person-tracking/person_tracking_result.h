// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PERSON_TRACKING_RESULT_H_
#define _PERSON_TRACKING_RESULT_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"
#include "person_info.h"

class PersonTrackingResult {
 public:
  PersonTrackingResult();

  PersonTrackingResult(const PersonTrackingResult& rhs);

  ~PersonTrackingResult();

  PersonTrackingResult& operator = (const PersonTrackingResult& rhs);

 public:
  ArrayHelper get_persons() const {
    return this->persons_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable ArrayHelperStorage persons_;
  friend class PersonTrackResultInternal;
};

#endif  // _PERSON_TRACKING_RESULT_H_
