// To add your copyright and license header

#ifndef _MAP_IMAGE_H_
#define _MAP_IMAGE_H_

#include <node.h>
#include <v8.h>

#include <vector>
#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

class MapImage {
 public:
  MapImage();

  MapImage(const MapImage& rhs) = delete;

  ~MapImage();

  MapImage& operator = (const MapImage& rhs);

 public:
  uint32_t get_width() const {
    return this->width_;
  }

  void set_width(uint32_t width) {
    this->width_ = width;
  }

  uint32_t get_height() const {
    return this->height_;
  }

  void set_height(uint32_t height) {
    this->height_ = height;
  }

  Uint8ArrayHelper get_data() const {
    return this->data_;
  }

  uint8_t* get_data_store(uint32_t width, uint32_t height) {
    data_store_.resize(4 * width * height);
    return data_store_.data();
  }

  void set_data_store(uint8_t* external_data, uint32_t width, uint32_t height) {
    this->data_store_.assign(external_data, external_data + width * height * 4);
  }

  void SetupTypedArray();

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  uint32_t width_;
  uint32_t height_;
  Uint8ArrayHelper data_;
  std::vector<uint8_t> data_store_;
};

// TODO(Donna): need to delete this class when deprecate the old implementation.
class ArgumentUnit {
 public:
  ArgumentUnit(bool draw_pose_trajectory, bool draw_occupancy_map) {
    this->draw_pose_trajectory = draw_pose_trajectory;
    this->draw_occupancy_map = draw_occupancy_map;
  }

 public:
  bool draw_pose_trajectory;
  bool draw_occupancy_map;
};
#endif  // _MAP_IMAGE_H_
