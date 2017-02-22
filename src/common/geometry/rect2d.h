// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_RECT2D_H_
#define _COMMON_GEOMETRY_RECT2D_H_

#include <node.h>
#include <v8.h>

#include <string>
#include <utility>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "common/geometry/point2d.h"
#include "common/geometry/range.h"
#include "common/geometry/size2d.h"

class Rect2D {
 public:
  Rect2D();

  explicit Rect2D(const double& x, const double& y,
                  const double& width, const double& height);
  explicit Rect2D(const Point2D& topLeft, const Size2D& size);
  explicit Rect2D(const ArrayHelper& coords);

  Rect2D(const Rect2D& rhs);

  ~Rect2D();

  Rect2D& operator = (const Rect2D& rhs);

 public:
  std::string ToString() const;

  double get_x() const {
    return x_;
  }

  void set_x(const double& value) {
    x_ = value;
    _overwriteOtherProperties();
  }

  double get_y() const {
    return y_;
  }

  void set_y(const double& value) {
    y_ = value;
    _overwriteOtherProperties();
  }

  double get_width() const {
    return width_;
  }

  void set_width(const double& value) {
    width_ = value;
    _overwriteOtherProperties();
  }

  double get_height() const {
    return height_;
  }

  void set_height(const double& value) {
    height_ = value;
    _overwriteOtherProperties();
  }

  double get_diagonalLength() const {
    return sqrt(get_width()*get_width() + height_*height_);
  }

  Size2D* get_size() const {
    return &size_;
  }

  void set_size(const Size2D& value) {
    width_ = value.get_width();
    height_ = value.get_height();
    _overwriteOtherProperties();
  }

  Point2D* get_topLeft() const {
    return &topLeft_;
  }

  void set_topLeft(const Point2D& value) {
    set_left(value.get_x());
    set_top(value.get_y());
    _overwriteOtherProperties();
  }

  Point2D* get_topRight() const {
    return &topRight_;
  }

  void set_topRight(const Point2D& value) {
    set_right(value.get_x());
    set_top(value.get_y());
    _overwriteOtherProperties();
  }

  Point2D* get_bottomLeft() const {
    return &bottomLeft_;
  }

  void set_bottomLeft(const Point2D& value) {
    set_left(value.get_x());
    set_bottom(value.get_y());
    _overwriteOtherProperties();
  }

  Point2D* get_bottomRight() const {
    return &bottomRight_;
  }

  void set_bottomRight(const Point2D& value) {
    set_right(value.get_x());
    set_bottom(value.get_y());
    _overwriteOtherProperties();
  }

  Point2D* get_center() const {
    return &center_;
  }

  void set_center(const Point2D& value) {
    width_ = (value.get_x() - x_) * 2;
    height_ = (value.get_y() - y_) * 2;
    _overwriteOtherProperties();
  }

  Point2D* get_topCenter() const {
    return &topCenter_;
  }

  void set_topCenter(const Point2D& value) {
    width_ = (value.get_x() - x_) * 2;
    set_y(value.get_y());
    _overwriteOtherProperties();
  }

  Point2D* get_bottomCenter() const {
    return &bottomCenter_;
  }

  void set_bottomCenter(const Point2D& value) {
    width_ = (value.get_x() - x_) * 2;
    set_y(value.get_y() - get_height());
    _overwriteOtherProperties();
  }

  Point2D* get_leftCenter() const {
    return &leftCenter_;
  }

  void set_leftCenter(const Point2D& value) {
    set_x(value.get_x());
    height_ = (value.get_y() - y_) * 2;
    _overwriteOtherProperties();
  }

  Point2D* get_rightCenter() const {
    return &rightCenter_;
  }

  void set_rightCenter(const Point2D& value) {
    set_x(value.get_x() - get_width());
    height_ = (value.get_y() - y_) * 2;
    _overwriteOtherProperties();
  }

  double get_top() const {
    return y_;
  }

  void set_top(const double& value) {
    height_ += y_ - value;
    y_ = value;
    _overwriteOtherProperties();
  }

  double get_left() const {
    return x_;
  }

  void set_left(const double& value) {
    width_ += x_ - value;
    x_ = value;
    _overwriteOtherProperties();
  }

  double get_bottom() const {
    return y_ + height_;
  }

  void set_bottom(const double& value) {
    height_ = value - get_y();
    _overwriteOtherProperties();
  }

  double get_right() const {
    return x_ + get_width();
  }

  void set_right(const double& value) {
    width_ = value - get_x();
    _overwriteOtherProperties();
  }

  Range* get_horzRange() const {
    return &horzRange_;
  }

  void set_horzRange(const Range& value) {
    set_left(value.get_begin());
    set_right(value.get_length());
  }

  Range* get_vertRange() const {
    return &vertRange_;
  }

  void set_vertRange(const Range& value) {
    set_top(value.get_begin());
    set_bottom(value.get_length());
  }

  bool equal(const Rect2D& rect) const;

  bool equal(const Point2D& topLeft, const Size2D& size) const;

  bool equal(const Point2D& topLeft, const Point2D& bottomRight) const;

  bool equal(const double& x, const double& y,
             const double& width, const double& height) const;

  bool equalCoord(const double& left, const double& top,
                  const double& right, const double& bottom) const;

  bool isEmpty();

  bool isValid();

  void adjust(const double& leftNudge, const double& topNudge,
              const double& rightNudge, const double& bottomNudge);

  void expand(const double& leftNudge, const double& topNudge,
              const double& rightNudge, const double& bottomNudge);

  void expand(const Size2D& size);

  void expand(const double& x, const double& y);

  void expand(const double& value);

  void shrink(const double& leftNudge, const double& topNudge,
              const double& rightNudge, const double& bottomNudge);

  void shrink(const Size2D& size);

  void shrink(const double& x, const double& y);

  void shrink(const double& value);

  void move(const double& x, const double& y);

  void moveTopTo(const double& coord);

  void moveLeftTo(const double& coord);

  void moveRightTo(const double& coord);

  void moveBottomTo(const double& coord);

  void moveTopLeftTo(const Point2D& point);

  void moveTopRightTo(const Point2D& point);

  void moveBottomRightTo(const Point2D& point);

  void moveBottomLeftTo(const Point2D& point);

  void moveCenterTo(const Point2D& point);

  void moveLeftCenterTo(const Point2D& point);

  void moveTopCenterTo(const Point2D& point);

  void moveRightCenterTo(const Point2D& point);

  void moveBottomCenterTo(const Point2D& point);

  void normalize();

  void setCoords(const double& left, const double& top,
                 const double& right, const double& bottom);

  void setCoords(const ArrayHelper& coords);

  ArrayHelper getCoords();

  bool onEdge(const double& x, const double& y);

  bool onEdge(const Point2D& point);

  bool contains(const Point2D& point);

  bool contains(const double& x, const double& y);

  bool contains(const Rect2D& rect);

  bool contains(const double& x, const double& y,
                const double& width, const double& height);

  bool containsCoord(const double& left, const double& top,
                     const double& right, const double& bottom);

  bool properlyContains(const Point2D& point);

  bool properlyContains(const double& x, const double& y);

  bool properlyContains(const Rect2D& rect);

  bool properlyContains(const double& x, const double& y,
                        const double& width, const double& height);

  bool properlyContainsCoord(const double& left, const double& top,
                            const double& right, const double& bottom);

  bool intersects(const Rect2D& rect);

  bool intersects(const double& x, const double& y,
                  const double& width, const double& height);

  bool intersectsCoord(const double& left, const double& top,
                       const double& right, const double& bottom);

  void unite(const Rect2D& rect);

  void unite(const double& x, const double& y,
             const double& width, const double& height);

  void uniteCoord(const double& left, const double& top,
                  const double& right, const double& bottom);

  ArrayHelper split(const double& x, const double& y);

  ArrayHelper split(const Point2D& point);

  ArrayHelper splitX(const double& x);

  ArrayHelper splitY(const double& y);

  Rect2D* united(const Rect2D& rect);

  Rect2D* united(const double& x, const double& y,
                 const double& width, const double& height);

  Rect2D* unitedCoord(const double& left, const double& top,
                      const double& right, const double& bottom);

  Rect2D* intersected(const Rect2D& rect);

  Rect2D* intersected(const double& x, const double& y,
                      const double& width, const double& height);

  Rect2D* intersectedCoord(const double& left, const double& top,
                           const double& right, const double& bottom);

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  double x_;

  double y_;

  double width_;

  double height_;

  mutable Size2D size_;

  mutable Point2D topLeft_;

  mutable Point2D topRight_;

  mutable Point2D bottomLeft_;

  mutable Point2D bottomRight_;

  mutable Point2D center_;

  mutable Point2D topCenter_;

  mutable Point2D bottomCenter_;

  mutable Point2D leftCenter_;

  mutable Point2D rightCenter_;

  mutable Range horzRange_;

  mutable Range vertRange_;

 private:
  void _init();

  void _overwriteOtherProperties() const {
    // Using x, y, width, height to overwrite other properties
    size_.quietlySetSize(width_, height_);
    topLeft_.quietlySetCoords(x_, y_);
    topRight_.quietlySetCoords(x_ + get_width(), y_);
    bottomLeft_.quietlySetCoords(x_, y_ + height_);
    bottomRight_.quietlySetCoords(x_ + get_width(), y_ + height_);
    center_.quietlySetCoords(x_ + get_width() / 2, y_ + height_ / 2);
    topCenter_.quietlySetCoords(center_.get_x(), get_top());
    bottomCenter_.quietlySetCoords(center_.get_x(), get_bottom());
    leftCenter_.quietlySetCoords(get_left(), center_.get_y());
    rightCenter_.quietlySetCoords(get_right(), center_.get_y());
    horzRange_.quietlySetRange(get_left(), get_right());
    vertRange_.quietlySetRange(get_top(), get_bottom());
  }
};

#endif  // _COMMON_GEOMETRY_RECT2D_H_
