// To add your copyright and license header

#include "person_recognizer_data_with_status.h"

PersonRecognizerDataWithStatus::PersonRecognizerDataWithStatus() {
  // TODO(widl-nan): init your members
}

PersonRecognizerDataWithStatus::PersonRecognizerDataWithStatus(
    const PersonRecognizerDataWithStatus& rhs) {
  data_ = rhs.data_;
  status_ = rhs.status_;
}

PersonRecognizerDataWithStatus::~PersonRecognizerDataWithStatus() {
  // TODO(widl-nan): do cleanup if necessary
}

PersonRecognizerDataWithStatus& PersonRecognizerDataWithStatus::operator = (
    const PersonRecognizerDataWithStatus& rhs) {
  if (&rhs != this) {
    data_ = rhs.data_;
    status_ = rhs.status_;
  }
  return *this;
}

