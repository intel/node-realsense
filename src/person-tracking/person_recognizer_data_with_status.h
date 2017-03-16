// To add your copyright and license header

#ifndef _PERSON_RECOGNIZER_DATA_WITH_STATUS_H_
#define _PERSON_RECOGNIZER_DATA_WITH_STATUS_H_

#include <node.h>
#include <v8.h>

#include <string>

#include "gen/generator_helper.h"
#include "gen/array_helper.h"

#include "person_recognizer_data.h"

class PersonRecognizerDataWithStatus {
 public:
  PersonRecognizerDataWithStatus();

  PersonRecognizerDataWithStatus(const PersonRecognizerDataWithStatus& rhs);

  ~PersonRecognizerDataWithStatus();

  PersonRecognizerDataWithStatus& operator = (
      const PersonRecognizerDataWithStatus& rhs);

 public:
  PersonRecognizerData* get_data() const {
    return &this->data_;
  }

  std::string get_status() const {
    return this->status_;
  }

  void SetJavaScriptThis(v8::Local<v8::Object> obj) {
    // Ignore this if you don't need it
    // Typical usage: emit an event on `obj`
  }

 private:
  mutable PersonRecognizerData data_;

  std::string status_;
  friend class PersonTrackerAdapter;
};

#endif  // _PERSON_RECOGNIZER_DATA_WITH_STATUS_H_
