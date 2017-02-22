// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OCCUPANCY_TILE_H_
#define _OCCUPANCY_TILE_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/array_helper.h"
#include "gen/generator_helper.h"

class OccupancyTile {
 public:
  OccupancyTile();

  OccupancyTile(const OccupancyTile& rhs);

  ~OccupancyTile();

  OccupancyTile& operator = (const OccupancyTile& rhs);

 public:
  int32_t get_x() const {
    return this->x_;
  }

  int32_t get_y() const {
    return this->y_;
  }

  int32_t get_occupancy() const {
    return this->occupancy_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t x_;

  int32_t y_;

  int32_t occupancy_;
};

#endif  // _OCCUPANCY_TILE_H_
