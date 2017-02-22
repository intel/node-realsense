// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _GEOMETRY_CHANGE_NOTIFY_H_
#define _GEOMETRY_CHANGE_NOTIFY_H_

typedef void GeometryChangeCallback(void*);

class GeometryChangeNotifyBase {
 public:
  GeometryChangeNotifyBase() {
    callback_ = nullptr;
    data_ = nullptr;
  }

  void RegisterChangeCallback(GeometryChangeCallback* callback, void* data) {
    callback_ = callback;
    data_ = data;
  }

  void NotifyChange() {
    if (callback_) {
      callback_(data_);
    }
  }

 private:
  GeometryChangeCallback* callback_;
  void* data_;
};

#endif  // _GEOMETRY_CHANGE_NOTIFY_H_
