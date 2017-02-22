// To add your copyright and license header

#include "map_image.h"

MapImage::MapImage()
    : width_(0), height_(0) {
}

MapImage::~MapImage() {
}

MapImage& MapImage::operator = (const MapImage& rhs) {
  if (&rhs != this) {
    // TODO(widl-nan): copy members from rhs
  }
  return *this;
}

void MapImage::SetupTypedArray() {
  SetupTypedArrayHelper(&data_,
    data_store_.data(),
    data_store_.size());
}
