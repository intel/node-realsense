// To add your copyright and license header

#ifndef _OCCUPANCY_MAP_BOUNDS_H_
#define _OCCUPANCY_MAP_BOUNDS_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class OccupancyMapBounds {
 public:
  OccupancyMapBounds();

  OccupancyMapBounds(const OccupancyMapBounds& rhs);

  ~OccupancyMapBounds();

  OccupancyMapBounds& operator = (const OccupancyMapBounds& rhs);

 public:
  int32_t get_minX() const {
    return this->minX_;
  }

  void set_minX(int32_t minx) {
    this->minX_ = minx;
  }

  int32_t get_minZ() const {
    return this->minZ_;
  }

  void set_minZ(int32_t minZ) {
    this->minZ_ = minZ;
  }

  int32_t get_maxX() const {
    return this->maxX_;
  }

  void set_maxX(int32_t maxX) {
    this->maxX_ = maxX;
  }

  int32_t get_maxZ() const {
    return this->maxZ_;
  }

  void set_maxZ(int32_t maxZ) {
    this->maxZ_ = maxZ;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  int32_t minX_;

  int32_t minZ_;

  int32_t maxX_;

  int32_t maxZ_;
};

#endif  // _OCCUPANCY_MAP_BOUNDS_H_
