// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
#ifndef _WORKER_PERSON_TRACKER_RUNNER_PROXY_H_
#define _WORKER_PERSON_TRACKER_RUNNER_PROXY_H_

#include <string>

#include "common/task/async_task_runner_instance.h"
#include "person_tracker_adapter.h"
#include "worker/person_tracking_tasks.h"

class PersonTrackerRunnerProxy {
 public:
  std::string GetStateString();
  void SetJavaScriptThis(v8::Local<v8::Object> obj);
  FrameData* GetFrameData();
  v8::Handle<v8::Promise> Start();
  v8::Handle<v8::Promise> Stop();
  v8::Handle<v8::Promise> Pause();
  v8::Handle<v8::Promise> Resume();
  v8::Handle<v8::Promise> Reset();
  v8::Handle<v8::Promise> SetPersonTrackerOptions(
      const PersonTrackerOptions& options);
  v8::Handle<v8::Promise> GetPersonTrackerOptions();
  v8::Handle<v8::Promise> SetCameraOptions(
      const CameraOptions& options);
  v8::Handle<v8::Promise> GetCameraOptions();
  v8::Handle<v8::Promise> StartTrackingPerson(const int32_t track_id);
  v8::Handle<v8::Promise> StopTrackingPerson(const int32_t track_id);
  v8::Handle<v8::Promise> ResetTracking();
  v8::Handle<v8::Promise> GetPersonInfo(const int32_t track_id);
  v8::Handle<v8::Promise> RegisterPerson(const int32_t track_id);
  v8::Handle<v8::Promise> UnRegisterPerson(const int32_t recognition_id);
  v8::Handle<v8::Promise> GetAllRecognitionIDs();
  v8::Handle<v8::Promise> RecognizePerson(const int32_t track_id);
  v8::Handle<v8::Promise> RecognizeAllPersons();
  v8::Handle<v8::Promise> RecognitionIDExist(const int32_t recognition_id);
  v8::Handle<v8::Promise> RemovePersonDescriptor(
      const int32_t recognition_id, const int32_t descriptor_id);
  v8::Handle<v8::Promise> GetPersonDescriptorIDs(const int32_t recognition_id);
  v8::Handle<v8::Promise> ReinforceRegistration(
      const int32_t tracking_id, const int32_t recognition_id);
  v8::Handle<v8::Promise> QuerySimilarityScore(
      const int32_t tracking_id, const int32_t recognition_id);
  v8::Handle<v8::Promise> ClearRecognitionDatabase();
  v8::Handle<v8::Promise> ExportRecognitionDatabase();
  v8::Handle<v8::Promise> ImportRecognitionDatabase(const ArrayBuffer& buf);
  // The below two method apply these options directly to adapter
  // without posting tasks.
  bool SetPersonTrackerOptionsDirectly(
      const DictionaryPersonTrackerOptions& options,
      std::string* fail_reason);
  bool SetCameraOptionsDirectly(
      const DictionaryCameraOptions& options,
      std::string* fail_reason);

  static PersonTrackerRunnerProxy* GetInstance();
  // The reference count is used to count the number of active js
  // PersonTracker objects, and the proxy is destroyed only when no
  // active PersonTracker objects.
  static void AddReference();
  static void RemoveReference();

 private:
  enum OperationType {
    kStart = 0,
    kPause,
    kResume,
    kReset,
    kStop,
    kGenericRunningOperation,  // generic operation in running state.
    kStartStopResetTrackingPerson,
    kSetOptions,
    kOperationCount
  };
  PersonTrackerRunnerProxy();
  ~PersonTrackerRunnerProxy();
  void CleanUp();
  bool PassStateCheck(OperationType operation, std::string* error);

  AsyncTaskRunner* runner_;
  PersonTrackerAdapter* adapter_;
  std::string pt_main_task_;
  static PersonTrackerRunnerProxy* proxy_;
  static int32_t reference_count_;
  static const char* state_check_matrix_[static_cast<uint32_t>(PersonTrackerAdapter::kStateCount)][static_cast<uint32_t>(kOperationCount)];  // NOLINT
  static const char* kNotStartError;
  static const char* kAlreadyStartError;
  static const char* kNotPausedError;
  static const char* kAlreadyPausedError;
  static const char* kResetError;
  static const char* kAlreadyConfiguredError;
};

#endif  // _WORKER_PERSON_TRACKER_RUNNER_PROXY_H_
