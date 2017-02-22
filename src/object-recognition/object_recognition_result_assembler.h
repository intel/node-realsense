// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OBJECT_RECOGNITION_RESULT_ASSEMBLER_H_
#define _OBJECT_RECOGNITION_RESULT_ASSEMBLER_H_

#include <or_data_interface.h>

#include <string>

#include "common/geometry/point3d.h"
#include "common/geometry/rect2d.h"
#include "common/type/v8_alias.h"

#include "object_recognition_label_name_translator.h"

using recognition_data_t   = rs::object_recognition::recognition_data;
using localization_data_t  = rs::object_recognition::localization_data;
using tracking_data_t      = rs::object_recognition::tracking_data;

class ObjectRecognitionResultAssembler {
 private:
  ObjectRecognitionResultAssembler();
  ~ObjectRecognitionResultAssembler();

 public:
  static v8_value_t AssembleSingleRecognitionResult(const std::string& label,
      double probability);

  static v8_value_t AssembleLocalizationResult(const std::string& label,
      double probability, const Rect2D& rect, const Point3D& pt);

  static v8_value_t AssembleTrackingResult(const Rect2D& roi,
      const Point3D& object_center);

  static v8_value_t AssembleSingleRecognitionResult(
      const recognition_data_t* data,
      size_t data_size);

  static v8_value_t AssembleLocalizationResult(
      const localization_data_t* data,
      size_t data_size);

  static v8_value_t AssembleTrackingResult(
      const tracking_data_t* data,
      size_t data_size);

  static void SetLabelNameTranslator(ObjectRecognitionLabelNameTranslator*);

  static ObjectRecognitionLabelNameTranslator* GetLabelNameTranslator();
};

#endif  // _OBJECT_RECOGNITION_RESULT_ASSEMBLER_H_
