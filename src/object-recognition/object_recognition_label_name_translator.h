// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OBJECT_RECOGNITION_LABEL_NAME_TRANSLATOR_H_
#define _OBJECT_RECOGNITION_LABEL_NAME_TRANSLATOR_H_

#include <string>

class ObjectRecognitionLabelNameTranslator {
 public:
  ObjectRecognitionLabelNameTranslator();
  virtual ~ObjectRecognitionLabelNameTranslator();

  std::string Translate(size_t id);

 private:
  virtual std::string DoTranslate(size_t id) = 0;

  class ObjectRecognitionLabelNameTranslatorD* d_;
};

#endif  // _OBJECT_RECOGNITION_LABEL_NAME_TRANSLATOR_H_
