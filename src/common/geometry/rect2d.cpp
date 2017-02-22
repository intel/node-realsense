// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "common/geometry/rect2d.h"

#include <cmath>
#include <sstream>

#include "common/geometry/float_equal.h"

#define DECLARE_CHANGE(prop) \
static void ApplyChangeFor_##prop(void* data) { \
  Rect2D* This = reinterpret_cast<Rect2D*>(data); \
  auto value = *This->get_##prop(); \
  This->set_##prop(value); \
}

#define REGISTER_CHANGE(prop, propVar) \
  propVar.RegisterChangeCallback(ApplyChangeFor_##prop, this);

DECLARE_CHANGE(topLeft);
DECLARE_CHANGE(topRight);
DECLARE_CHANGE(bottomLeft);
DECLARE_CHANGE(bottomRight);
DECLARE_CHANGE(center);
DECLARE_CHANGE(leftCenter);
DECLARE_CHANGE(topCenter);
DECLARE_CHANGE(rightCenter);
DECLARE_CHANGE(bottomCenter);
DECLARE_CHANGE(horzRange);
DECLARE_CHANGE(vertRange);

void Rect2D::_init() {
  // topLeft_.RegisterChangeCallback(ApplyChangeFor_topLeft, this);
  // topRight_.RegisterChangeCallback(ApplyChangeFor_topRight, this);

  REGISTER_CHANGE(topLeft, topLeft_);
  REGISTER_CHANGE(topRight, topRight_);
  REGISTER_CHANGE(bottomLeft, bottomLeft_);
  REGISTER_CHANGE(bottomRight, bottomRight_);
  REGISTER_CHANGE(center, center_);
  REGISTER_CHANGE(leftCenter, leftCenter_);
  REGISTER_CHANGE(topCenter, topCenter_);
  REGISTER_CHANGE(rightCenter, rightCenter_);
  REGISTER_CHANGE(bottomCenter, bottomCenter_);
  REGISTER_CHANGE(horzRange, horzRange_);
  REGISTER_CHANGE(vertRange, vertRange_);
}

Rect2D::Rect2D() {
  x_ = y_ = width_ = height_ = 0;
  _overwriteOtherProperties();

  _init();
}

Rect2D::Rect2D(const double& x, const double& y,
               const double& width, const double& height) {
  x_ = x;
  y_ = y;
  width_ = width;
  height_ = height;
  _overwriteOtherProperties();

  _init();
}

Rect2D::Rect2D(const Point2D& topLeft, const Size2D& size) {
  x_ = topLeft.get_x();
  y_ = topLeft.get_y();
  width_ = size.get_width();
  height_ = size.get_height();
  _overwriteOtherProperties();

  _init();
}

Rect2D::Rect2D(const ArrayHelper& coords) {
  x_ = y_ = width_ = height_ = 0;
  setCoords(coords);

  _init();
}

Rect2D::Rect2D(const Rect2D& rhs) {
  x_ = rhs.x_;
  y_ = rhs.y_;
  width_ = rhs.get_width();
  height_ = rhs.height_;
  _overwriteOtherProperties();

  _init();
}

Rect2D::~Rect2D() {
}

Rect2D& Rect2D::operator = (const Rect2D& rhs) {
  if (&rhs != this) {
    x_ = rhs.x_;
    y_ = rhs.y_;
    width_ = rhs.get_width();
    height_ = rhs.height_;
    _overwriteOtherProperties();
  }
  return *this;
}

bool Rect2D::equal(const Rect2D& rect) const {
  return equal(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

bool Rect2D::equal(const Point2D& topLeft, const Size2D& size) const {
  return equal(topLeft.get_x(), topLeft.get_y(),
               size.get_width(), size.get_height());
}

bool Rect2D::equal(const Point2D& topLeft, const Point2D& bottomRight) const {
  return equalCoord(topLeft.get_x(), topLeft.get_y(),
                    bottomRight.get_x(), bottomRight.get_y());
}

bool Rect2D::equal(const double& x, const double& y,
                   const double& width, const double& height) const {
  return double_equal(x_, x) && double_equal(y_, y) &&
    double_equal(get_width(), width) && double_equal(height_, height);
}

bool Rect2D::equalCoord(const double& left, const double& top,
                        const double& right, const double& bottom) const {
  _overwriteOtherProperties();
  return double_equal(get_left(), left) && double_equal(get_top(), top) &&
    double_equal(get_right(), right) && double_equal(get_bottom(), bottom);
}

bool Rect2D::isEmpty() {
  return get_width() > 0 && get_height() > 0;
}

bool Rect2D::isValid() {
  return !std::isnan(x_) && !std::isnan(y_) &&
    !std::isnan(get_width()) && !std::isnan(height_) &&
    get_width() >= 0 && get_height() >= 0;
}

void Rect2D::adjust(const double& leftNudge, const double& topNudge,
                    const double& rightNudge, const double& bottomNudge) {
  set_right(get_right() + rightNudge);
  set_bottom(get_bottom() + bottomNudge);
  set_left(get_left() + leftNudge);
  set_top(get_top() + topNudge);
  _overwriteOtherProperties();
}

void Rect2D::expand(const double& leftNudge, const double& topNudge,
                    const double& rightNudge, const double& bottomNudge) {
  adjust(-leftNudge, -topNudge, rightNudge, bottomNudge);
}

void Rect2D::expand(const Size2D& size) {
  adjust(size.get_width(), size.get_height(),
         size.get_width(), size.get_height());
}

void Rect2D::expand(const double& x, const double& y) {
  adjust(x, y, x, y);
}

void Rect2D::expand(const double& value) {
  adjust(value, value, value, value);
}

void Rect2D::shrink(const double& leftNudge, const double& topNudge,
                    const double& rightNudge, const double& bottomNudge) {
  adjust(leftNudge, topNudge, -rightNudge, -bottomNudge);
}

void Rect2D::shrink(const Size2D& size) {
  adjust(-size.get_width(), -size.get_height(),
         -size.get_width(), -size.get_height());
}

void Rect2D::shrink(const double& x, const double& y) {
  adjust(-x, -y, -x, -y);
}

void Rect2D::shrink(const double& value) {
  adjust(-value, -value, -value, -value);
}

void Rect2D::move(const double& x, const double& y) {
  x_ += x;
  y_ += y;
  _overwriteOtherProperties();
}

void Rect2D::moveTopTo(const double& coord) {
  y_ = coord;
  _overwriteOtherProperties();
}

void Rect2D::moveLeftTo(const double& coord) {
  x_ = coord;
  _overwriteOtherProperties();
}

void Rect2D::moveRightTo(const double& coord) {
  x_ = coord - get_width();
  _overwriteOtherProperties();
}

void Rect2D::moveBottomTo(const double& coord) {
  y_ = coord - height_;
  _overwriteOtherProperties();
}

void Rect2D::moveTopLeftTo(const Point2D& point) {
  moveTopTo(point.get_y());
  moveLeftTo(point.get_x());
}

void Rect2D::moveTopRightTo(const Point2D& point) {
  moveTopTo(point.get_y());
  moveRightTo(point.get_x());
}

void Rect2D::moveBottomRightTo(const Point2D& point) {
  moveBottomTo(point.get_y());
  moveRightTo(point.get_x());
}

void Rect2D::moveBottomLeftTo(const Point2D& point) {
  moveBottomTo(point.get_y());
  moveLeftTo(point.get_x());
}

void Rect2D::moveCenterTo(const Point2D& point) {
  set_x(point.get_x() - get_width() / 2);
  set_y(point.get_y() - get_height() / 2);
  _overwriteOtherProperties();
}

void Rect2D::moveLeftCenterTo(const Point2D& point) {
  set_x(point.get_x());
  set_y(point.get_y() - get_height() / 2);
  _overwriteOtherProperties();
}

void Rect2D::moveTopCenterTo(const Point2D& point) {
  set_x(point.get_x() - get_width() / 2);
  set_y(point.get_y());
  _overwriteOtherProperties();
}

void Rect2D::moveRightCenterTo(const Point2D& point) {
  set_right(point.get_x());
  set_y(point.get_y() - get_height() / 2);
  _overwriteOtherProperties();
}

void Rect2D::moveBottomCenterTo(const Point2D& point) {
  set_x(point.get_x() - get_width() / 2);
  set_bottom(point.get_y());
  _overwriteOtherProperties();
}

void Rect2D::normalize() {
  if (get_width() < 0) {
    x_ = x_ + get_width();
    width_ = - get_width();
  }

  if (height_ < 0) {
    y_ = y_ + height_;
    height_ = - height_;
  }

  _overwriteOtherProperties();
  // TODO(widl-nan): fill your code here
}

void Rect2D::setCoords(const double& left, const double& top,
                       const double& right, const double& bottom) {
  x_ = left;
  y_ = top;
  width_ = right - left;
  height_ = bottom - top;
  _overwriteOtherProperties();
}

void Rect2D::setCoords(const ArrayHelper& coords) {
  if (coords.Length() > 3) {
    setCoords(coords.GetFloat(0), coords.GetFloat(1),
              coords.GetFloat(2), coords.GetFloat(3));
  }
  _overwriteOtherProperties();
}

ArrayHelper Rect2D::getCoords() {
  ArrayHelper a;
  a.SetT(0, get_left());
  a.SetT(1, get_top());
  a.SetT(2, get_right());
  a.SetT(3, get_bottom());
  return a;
}

bool Rect2D::onEdge(const double& x, const double& y) {
  if (x >= get_left() && x <= get_right()) {
    return y == get_top() || y == get_bottom();
  }
  if (y >= get_top() && y <= get_bottom()) {
    return x == get_left() || x == get_right();
  }
  return false;
}

bool Rect2D::onEdge(const Point2D& point) {
  return onEdge(point.get_x(), point.get_y());
}

bool Rect2D::contains(const Point2D& point) {
  return contains(point.get_x(), point.get_y());
}

bool Rect2D::contains(const double& x, const double& y) {
  return x >= get_left() && x <= get_right() &&
         y >= get_top() && y <= get_bottom();
}

bool Rect2D::contains(const Rect2D& rect) {
  return contains(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

bool Rect2D::contains(const double& x, const double& y,
                      const double& width, const double& height) {
  return containsCoord(x, y, x + width, y + height);
}

bool Rect2D::containsCoord(const double& left, const double& top,
                           const double& right, const double& bottom) {
  return left >= get_left() && left <= get_right() &&
    top >= get_top() && top <= get_bottom() &&
    right <= get_right() && right >= get_left() &&
    bottom <= get_bottom() && bottom >= get_top();
}

bool Rect2D::properlyContains(const Point2D& point) {
  return properlyContains(point.get_x(), point.get_y());
}

bool Rect2D::properlyContains(const double& x, const double& y) {
  return x > get_left() && x < get_right() &&
         y > get_top() && y < get_bottom();
}

bool Rect2D::properlyContains(const Rect2D& rect) {
  return properlyContains(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

bool Rect2D::properlyContains(const double& x, const double& y,
                              const double& width, const double& height) {
  return properlyContainsCoord(x, y, x + width, y + height);
}

bool Rect2D::properlyContainsCoord(const double& left, const double& top,
                                   const double& right, const double& bottom) {
  return left > get_left() && left < get_right() &&
    top > get_top() && top < get_bottom() &&
    right < get_right() && right > get_left() &&
    bottom < get_bottom() && bottom > get_top();
}

bool Rect2D::intersects(const Rect2D& rect) {
  return intersects(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

bool Rect2D::intersects(const double& x, const double& y,
                        const double& width, const double& height) {
  return intersectsCoord(x, y, x + width, y + height);
}

bool Rect2D::intersectsCoord(const double& left, const double& top,
                             const double& right, const double& bottom) {
  // TODO(widl-nan): fill your code here
}

void Rect2D::unite(const Rect2D& rect) {
  return unite(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

void Rect2D::unite(const double& x, const double& y,
                   const double& width, const double& height) {
  return uniteCoord(x, y, x + width, y + height);
}

void Rect2D::uniteCoord(const double& left, const double& top,
                        const double& right, const double& bottom) {
  // TODO(widl-nan): fill your code here
}

ArrayHelper Rect2D::split(const double& x, const double& y) {
  // TODO(widl-nan): fill your code here
}

ArrayHelper Rect2D::split(const Point2D& point) {
  return split(point.get_x(), point.get_y());
}

ArrayHelper Rect2D::splitX(const double& x) {
  return split(x, y_ - 1);
}

ArrayHelper Rect2D::splitY(const double& y) {
  return split(x_ - 1, y);
}

Rect2D* Rect2D::united(const Rect2D& rect) {
  return united(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

Rect2D* Rect2D::united(const double& x, const double& y,
                       const double& width, const double& height) {
  return unitedCoord(x, y, x + width, y + height);
}

Rect2D* Rect2D::unitedCoord(const double& left, const double& top,
                            const double& right, const double& bottom) {
  // TODO(widl-nan): fill your code here
}

Rect2D* Rect2D::intersected(const Rect2D& rect) {
  return intersected(rect.x_, rect.y_, rect.get_width(), rect.height_);
}

Rect2D* Rect2D::intersected(const double& x, const double& y,
                            const double& width, const double& height) {
  return intersectedCoord(x, y, x + width, y + height);
}

Rect2D* Rect2D::intersectedCoord(const double& left, const double& top,
                                 const double& right, const double& bottom) {
  // TODO(widl-nan): fill your code here
}

std::string Rect2D::ToString() const {
  std::stringstream ss;
  ss << "Rect2D {";
  ss << "x: " << get_x() << ", ";
  ss << "y: " << get_y() << ", ";
  ss << "width: " << get_width() << ", ";
  ss << "height: " << get_height();
  ss << "}";
  return ss.str();
}
