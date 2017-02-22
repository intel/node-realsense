// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "occupancy_tile.h"

OccupancyTile::OccupancyTile() {
  // TODO(widl-nan): init your members
}

OccupancyTile::OccupancyTile(const OccupancyTile& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in occupancy_tile.h
}

OccupancyTile::~OccupancyTile() {
  // TODO(widl-nan): do cleanup if necessary
}

OccupancyTile& OccupancyTile::operator = (const OccupancyTile& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}
