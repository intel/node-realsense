// To add your copyright and license header

#ifndef _OCCUPANCY_MAP_DATA_H_
#define _OCCUPANCY_MAP_DATA_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "default_values.h"
#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class OccupancyMapData {
 public:
  OccupancyMapData();

  OccupancyMapData(const OccupancyMapData& rhs);

  ~OccupancyMapData();

  OccupancyMapData& operator = (const OccupancyMapData& rhs);

 public:
  bool get_isDataPending() const {
    return this->is_data_pending_;
  }

  int32_t get_tileCount() const {
    return this->tile_count_;
  }

  Int32ArrayHelper get_data() const {
    return this->data_;
  }

  void SetData(bool is_data_pending, int32_t tile_count, const int32_t* data);

  void SetupTypedArray();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  bool is_data_pending_;
  int32_t tile_count_;
  Int32ArrayHelper data_;
  int32_t* data_store_;
};

#endif  // _OCCUPANCY_MAP_DATA_H_
