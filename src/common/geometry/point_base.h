// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _COMMON_GEOMETRY_POINT_BASE_H_
#define _COMMON_GEOMETRY_POINT_BASE_H_

#include "common/geometry/float_equal.h"
#include "common/geometry/geometry_change_notify.h"

class Point2DBase : public GeometryChangeNotifyBase {
 public:
  Point2DBase() {
    x_ = y_ = 0;
  }

  explicit Point2DBase(const double& x, const double& y) {
    setCoords(x, y);
  }

  Point2DBase(const Point2DBase& rhs) {
    x_ = rhs.x_;
    y_ = rhs.y_;
  }

  Point2DBase& operator = (const Point2DBase& rhs) {
    if (&rhs != this) {
      x_ = rhs.x_;
      y_ = rhs.y_;
    }
    return *this;
  }

 public:
  double get_x() const {return this->x_; }
  void set_x(const double& new_value) {this->x_ = new_value; NotifyChange(); }

  double get_y() const {return this->y_; }
  void set_y(const double& new_value) {this->y_ = new_value; NotifyChange(); }

  void setCoords(const double& x, const double& y) {
    x_ = x;
    y_ = y;
    NotifyChange();
  }

  void quietlySetCoords(double x, double y) {
    x_ = x;
    y_ = y;
  }

  bool equal(const Point2DBase& point) {
    return equal(point.x_, point.y_);
  }

  bool equal(const double& x, const double& y) {
    return double_equal(x_, x) && double_equal(y_, y);
  }

  static double distance(const Point2DBase& p1, const Point2DBase& p2);

  static double distance(const double& x1, const double& y1,
                         const double& x2, const double& y2);

 protected:
  double x_;
  double y_;
};

class Point3DBase {
 public:
  Point3DBase() {
    x_ = y_ = z_ = 0;
  }

  explicit Point3DBase(const double& x, const double& y, const double& z) {
    x_ = x;
    y_ = y;
    z_ = z;
  }

  Point3DBase(const Point3DBase& rhs) {
    x_ = rhs.x_;
    y_ = rhs.y_;
    z_ = rhs.z_;
  }

  ~Point3DBase() {}

  Point3DBase& operator = (const Point3DBase& rhs) {
    if (this != &rhs) {
      x_ = rhs.x_;
      y_ = rhs.y_;
      z_ = rhs.z_;
    }
    return *this;
  }

 public:
  double get_x() const {return this->x_; }
  void set_x(const double& new_value) {this->x_ = new_value; }

  double get_y() const {return this->y_; }
  void set_y(const double& new_value) {this->y_ = new_value; }

  double get_z() const {return this->z_; }
  void set_z(const double& new_value) {this->z_ = new_value; }

  void setCoords(const double& x, const double& y, const double& z) {
    x_ = x;
    y_ = y;
    z_ = z;
  }

  bool equal(const Point3DBase& point) {
    return equal(point.x_, point.y_, point.z_);
  }

  bool equal(const double& x, const double& y, const double& z) {
    return double_equal(x_, x) && double_equal(y_, y) && double_equal(z_, z);
  }

  static double distance(const Point3DBase& p1, const Point3DBase& p2);

  static double distance(const double& x1,
                         const double& y1,
                         const double& z1,
                         const double& x2,
                         const double& y2,
                         const double& z2);

 protected:
  double x_;

  double y_;

  double z_;
};

#endif  // _COMMON_GEOMETRY_POINT_BASE_H_
