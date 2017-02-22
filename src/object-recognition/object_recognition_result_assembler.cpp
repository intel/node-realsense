// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "object_recognition_result_assembler.h"

#include "gen/nan__localization_info.h"
#include "gen/nan__tracking_info.h"
#include "gen/nan__recognition_info.h"

class ResultAssemblerD {
  friend class ObjectRecognitionResultAssembler;
  static ObjectRecognitionLabelNameTranslator* translator;

  static std::string Translate(size_t id) {
    if (!translator) {
      return "<translator-not-found>";
    }
    return translator->Translate(id);
  }
};

ObjectRecognitionLabelNameTranslator* ResultAssemblerD::translator = nullptr;

v8_value_t ObjectRecognitionResultAssembler::AssembleSingleRecognitionResult(
    const std::string& label, double probability) {
  return NanRecognitionInfo::NewInstance(
      new RecognitionInfo(label, probability));
}

v8_value_t ObjectRecognitionResultAssembler::AssembleLocalizationResult(
    const std::string& label, double probability,
    const Rect2D& rect, const Point3D& pt) {
  LocalizationInfo* info = new LocalizationInfo(label, probability, rect, pt);
  return NanLocalizationInfo::NewInstance(info);
}

v8_value_t ObjectRecognitionResultAssembler::AssembleTrackingResult(
    const Rect2D& roi, const Point3D& object_center) {
  return NanTrackingInfo::NewInstance(
      new TrackingInfo(roi, object_center));
}

v8_value_t ObjectRecognitionResultAssembler::AssembleSingleRecognitionResult(
    const recognition_data_t* data,
    size_t data_size) {
  ArrayHelper array_generator;
  for (size_t i = 0; i < data_size; ++i) {
    std::string label = ResultAssemblerD::Translate(data[i].label);
    auto probability = data[i].probability;
    RecognitionInfo* info = new RecognitionInfo(label, probability);
    array_generator.Set(i, NanRecognitionInfo::NewInstance(info));
  }
  return static_cast<v8_array_t>(array_generator);
}

v8_value_t ObjectRecognitionResultAssembler::AssembleLocalizationResult(
    const localization_data_t* data,
    size_t data_size) {
  ArrayHelper array_generator;
  for (size_t i = 0; i < data_size; ++i) {
    std::string label = ResultAssemblerD::Translate(data[i].label);
    auto probability = data[i].probability;
    Rect2D roi(data[i].roi.x, data[i].roi.y,
        data[i].roi.width, data[i].roi.height);
    Point3D object_center(data[i].object_center.coordinates.x,
        data[i].object_center.coordinates.y,
        data[i].object_center.coordinates.z);
    LocalizationInfo* info = new LocalizationInfo(label,
                                                  probability,
                                                  roi,
                                                  object_center);
    array_generator.Set(i, NanLocalizationInfo::NewInstance(info));
  }
  return static_cast<v8_array_t>(array_generator);
}

v8_value_t ObjectRecognitionResultAssembler::AssembleTrackingResult(
    const tracking_data_t* data,
    size_t data_size) {
  ArrayHelper array_generator;
  for (size_t i = 0; i < data_size; ++i) {
    Rect2D roi(data[i].roi.x, data[i].roi.y,
        data[i].roi.width, data[i].roi.height);
    Point3D object_center(data[i].object_center.coordinates.x,
        data[i].object_center.coordinates.y,
        data[i].object_center.coordinates.z);
    array_generator.Set(i,
        NanTrackingInfo::NewInstance(new TrackingInfo(roi, object_center)));
  }
  return static_cast<v8_array_t>(array_generator);
}

void ObjectRecognitionResultAssembler::SetLabelNameTranslator(
    ObjectRecognitionLabelNameTranslator* translator) {
  ResultAssemblerD::translator = translator;
}

ObjectRecognitionLabelNameTranslator*
    ObjectRecognitionResultAssembler::GetLabelNameTranslator() {
  return ResultAssemblerD::translator;
}
