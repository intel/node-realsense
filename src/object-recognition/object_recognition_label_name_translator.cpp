// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include "object_recognition_label_name_translator.h"

class ObjectRecognitionLabelNameTranslatorD {
  ObjectRecognitionLabelNameTranslatorD();
  ~ObjectRecognitionLabelNameTranslatorD();
  friend class ObjectRecognitionLabelNameTranslator;
};

ObjectRecognitionLabelNameTranslatorD::ObjectRecognitionLabelNameTranslatorD() {  // NOLINT(*)
}

ObjectRecognitionLabelNameTranslatorD::~ObjectRecognitionLabelNameTranslatorD() {  // NOLINT(*)
}

ObjectRecognitionLabelNameTranslator::ObjectRecognitionLabelNameTranslator() {
  d_ = new ObjectRecognitionLabelNameTranslatorD();
}

ObjectRecognitionLabelNameTranslator::~ObjectRecognitionLabelNameTranslator() {
  delete d_;
}

std::string ObjectRecognitionLabelNameTranslator::Translate(size_t id) {
  return DoTranslate(id);
}
