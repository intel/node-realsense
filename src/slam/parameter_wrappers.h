// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _PARAMETER_WRAPPERS_H_
#define _PARAMETER_WRAPPERS_H_

#include <string>

#include "gen/region_of_interest.h"
#include "map_image.h"
#include "occupancy_map_data.h"

class ParameterWrapperForSavingPpmMap {
 public:
  ParameterWrapperForSavingPpmMap(
      std::string file_name, bool draw_camera_trajectory) {
    this->file_name = file_name;
    this->draw_camera_trajectory = draw_camera_trajectory;
  }

 public:
  std::string file_name;
  bool draw_camera_trajectory;
};

class ParameterWrapperForGetRgbaMap {
 public:
  ParameterWrapperForGetRgbaMap(
      bool draw_pose_trajectory, bool draw_occupancy_map) {
    this->draw_pose_trajectory = draw_pose_trajectory;
    this->draw_occupancy_map = draw_occupancy_map;
    this->map_image = nullptr;
  }

 public:
  bool draw_pose_trajectory;
  bool draw_occupancy_map;
  MapImage* map_image;
};

class ParameterWrapperForOccupancyMap {
 public:
  explicit ParameterWrapperForOccupancyMap(const RegionOfInterest* roi) {
    has_roi = false;
    if (roi != nullptr) {
      has_roi = true;
      this->roi.ImportFrom(*roi);
    }
    this->map_data = new OccupancyMapData();
  }

  ~ParameterWrapperForOccupancyMap() {
    // Can't delete map_data here. It will be used in JS context.
  }

 public:
  bool has_roi;
  DictionaryRegionOfInterest roi;
  OccupancyMapData* map_data;
};

#endif  // _PARAMETER_WRAPPERS_H_
