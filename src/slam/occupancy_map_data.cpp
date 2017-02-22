// To add your copyright and license header

#include "occupancy_map_data.h"

OccupancyMapData::OccupancyMapData()
    : is_data_pending_(false), tile_count_(0), data_store_(nullptr) {
}

OccupancyMapData::OccupancyMapData(const OccupancyMapData& rhs) {
  // TODO(widl-nan): copy from rhs if you want this behavior
  // Or mark ctor = delete in occupancy_map_data.h
}

OccupancyMapData::~OccupancyMapData() {
  if (data_store_) delete [] data_store_;
}

OccupancyMapData& OccupancyMapData::operator = (const OccupancyMapData& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

void OccupancyMapData::SetData(bool is_data_pending, int32_t tile_count,
                               const int32_t* data) {
  is_data_pending_ = is_data_pending;
  tile_count_ = tile_count;
  if (data_store_ != nullptr) delete data_store_;

  data_store_ = new int32_t[3 * tile_count_];
  memcpy(data_store_, data, 3 * tile_count_);
}

void OccupancyMapData::SetupTypedArray() {
  SetupTypedArrayHelper(&data_,
    data_store_,
    tile_count_ * 3);
}
