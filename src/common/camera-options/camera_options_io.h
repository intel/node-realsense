// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _CAMERA_OPTIONS_IO_H_
#define _CAMERA_OPTIONS_IO_H_

#include <string>
#include <vector>
#include <algorithm>

#include "camera_options_type.h"

class CameraOptionsIO {
  CameraOptionsIO(const CameraOptionsIO&) = delete;
  CameraOptionsIO& operator = (const CameraOptionsIO&) = delete;

 public:
  CameraOptionsIO() {}

  // Actually nobody will delete a pointer using this interface
  virtual ~CameraOptionsIO() {}

  // Return true if you can support this options set
  virtual bool IsSupported(const CameraOptionsType* src) const = 0;

  // Tell me which item(s) you can't support
  virtual std::string GetUnsupportedMessage() const = 0;

  // Take it in, only after IsSupported() == true
  // Throw exception for any error that you hit
  virtual void ApplyToCamera(const CameraOptionsType* src) = 0;

  // Read camera data/readings and fill in relevant members in `target`
  //  Note the member-filling partially, leaving rest of the part untouched.
  virtual void PartiallyFillInCameraData(CameraOptionsType* target) = 0;
};

class CameraOptionsIOComposite : public CameraOptionsIO {
 public:
  CameraOptionsIOComposite() {
  }

  ~CameraOptionsIOComposite() {
    Clear();
  }

  inline void Add(CameraOptionsIO* child) {
    if (!Exists(child)) {
      children_.push_back(child);
    }
  }

  inline void Remove(CameraOptionsIO* child) {
    auto pos = std::remove(children_.begin(), children_.end(), child);
    children_.erase(pos, children_.end());
  }

  inline bool Exists(CameraOptionsIO* child) {
    auto pos = std::find(children_.begin(), children_.end(), child);
    return pos != children_.end();
  }

  inline void Clear() {
    children_.clear();
  }

  virtual bool IsSupported(const CameraOptionsType* src) const {
    for (auto it = children_.begin();
        it != children_.end();
        ++it) {
      const CameraOptionsIO* ptr = *it;
      if (!ptr->IsSupported(src)) {
        return false;
      }
    }
    return true;
  }

  virtual std::string GetUnsupportedMessage() const {
    std::string message;
    for (auto it = children_.begin();
        it != children_.end();
        ++it) {
      const CameraOptionsIO* ptr = *it;
      message += ptr->GetUnsupportedMessage();
    }
    return message;
  }

  virtual void ApplyToCamera(const CameraOptionsType* src) {
    for (auto it = children_.begin();
        it != children_.end();
        ++it) {
      CameraOptionsIO* ptr = *it;
      ptr->ApplyToCamera(src);
    }
  }

  virtual void PartiallyFillInCameraData(CameraOptionsType* target) {
    for (auto it = children_.begin();
        it != children_.end();
        ++it) {
      CameraOptionsIO* ptr = *it;
      ptr->PartiallyFillInCameraData(target);
    }
  }

 private:
  std::vector<CameraOptionsIO*> children_;
};

#endif  // _CAMERA_OPTIONS_IO_H_
