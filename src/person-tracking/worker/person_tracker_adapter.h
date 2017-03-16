// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _WORKER_PERSON_TRACKING_ADAPTER_H_
#define _WORKER_PERSON_TRACKING_ADAPTER_H_

#include <RealSense/Image.h>
// TODO(halton): PersonTrackingData.h must be included after
// PersonTrackingModule.h. Need figure out the root case
#include <RealSense/PersonTracking/PersonTrackingModule.h>
#include <RealSense/PersonTracking/PersonTrackingData.h>
#include <librealsense/rs.hpp>
#include <person_tracking_video_module_factory.h>
#include <rs_sdk.h>
#include <uv.h>
#include <v8.h>

#include <memory>
#include <mutex> //NOLINT
#include <string>
#include <vector>

#include "common/camera-options/camera_options_io.h"
#include "common/frame_data.h"
#include "common/camera-delegate/camera_delegate_instance.h"
#include "gen/camera_options.h"
#include "gen/person_tracker_options.h"
#include "gen/nan__person_tracking_result.h"
#include "person_recognizer_data_with_status.h"
#include "person_registration_data.h"
#include "worker/person_tracking_config.h"
#include "worker/person_tracking_result_internal.h"
#include "worker/utils.h"

namespace RS = Intel::RealSense;
namespace PT = RS::PersonTracking;
using PTDATA = PT::PersonTrackingData;
using PTRECOGNITION = PT::PersonTrackingData::PersonRecognition;

class PersonTrackerAdapter : public CameraOptionsIO {
 public:
  enum PersonTrackerState {
    kStateReady = 0,
    kStateRunning,
    kStateDetecting,
    kStateTracking,
    kStatePaused,
    kStateError,
    // kStateCount is only used to identify the number of state,
    // should never be used to identify a state.
    kStateCount
  };

  struct PesonTrackerStateMap {
    PersonTrackerState state;
    std::string state_string;
  };

  class SafeGuard {
   public:
    SafeGuard() {
      PersonTrackerAdapter::Instance()->lock();
    }
    ~SafeGuard() {
      PersonTrackerAdapter::Instance()->unlock();
    }
  };

  virtual ~PersonTrackerAdapter();
  bool SetConfig(const PersonTrackerOptions& cfg, std::string* error);
  bool SetConfig(
      std::shared_ptr<DictionaryPersonTrackerOptions> cfg, std::string* error);
  bool HaveConfig() { return config_.option_helper_.options_?true:false; }
  // This method create v8 objects, should be called in v8 main thread.
  bool GetConfig(PersonTrackerOptions* options);
  bool ConfigureCamera();
  bool ConfigureModule();
  bool Start();
  bool IterationWork();
  void lock();
  void unlock();
  void Stop();
  void Pause();
  void Resume();
  void Reset();
  bool StartTracking(int32_t track_id);
  bool StopTracking(int32_t track_id);
  bool RegisterPerson(
      int32_t track_id, PersonRegistrationData* result, std::string* err);
  bool ReinforceRegistration(int32_t track_id, int32_t recognition_id,
      PersonRegistrationData* result, std::string* err);
  bool UnRegisterPerson(int32_t recognition_id, std::string* err);
  bool RecognitionIDExist(
      int32_t recognition_id, bool* exist, std::string* err);
  bool GetRecognitionIDs(std::vector<int32_t>* id_vec, std::string* err);
  bool GetRegistrationDescriptorIDs(
      int32_t recognition_id, std::vector<int32_t>* id_vec, std::string* err);
  bool RemoveRegistrationDescriptor(
      int32_t recognition_id, int32_t descriptor_id, std::string* err);
  bool RecognizePerson(
      int32_t track_id, PersonRecognizerData* result, std::string* err);
  bool RecognizeAllPersons(
      std::vector<PersonRecognizerDataWithStatus*>* result_vec,
      std::string* err);
  bool QueryRecognitionSimilarityScore(
      int32_t track_id, int32_t recognition_id, float* score,
      std::string* err);
  bool ClearRecognitionDatabase(std::string* err);
  bool ExportRecognitionDatabase(
      int32_t* size, unsigned char** buf, std::string* err);
  bool ImportRecognitionDatabase(
      int32_t size, unsigned char* buf, std::string* err);
  std::string GetRegistrationErrDescription(
      PTRECOGNITION::RegistrationStatus status);
  std::string GetRecognitionErrDescription(
      PTRECOGNITION::RecognitionStatus status);
  void ResetJSObject();
  void SetJavaScriptThis(v8::Local<v8::Object> obj);
  v8::Local<v8::Object> GetJavaScriptThis();
  void BeforeEmitResult();
  void AfterEmitResult();
  bool ShouldPopEvent(size_t event_index);
  size_t GetEventCount();
  std::string GetEventName(size_t event_index);
  v8::Local<v8::Value> PopEventData(size_t event_index);
    FrameData* GetFrameData();
  bool HasValidPersonData();
  std::string GetStateString() {
    return state_maps_[static_cast<int32_t>(state_)].state_string;
  }
  PersonTrackerState GetState() { return state_; }
  std::shared_ptr<PersonDataInternal> GetPersonData(int32_t trackID);
  std::shared_ptr<DictionaryCameraOptions> GetCameraOptions() {
    return config_.camera_options_;
  }
  bool ModuleConfigured() { return module_configured_; }

  // CameraOptionsIO methods
  virtual bool IsSupported(const CameraOptionsType* src) const;
  virtual std::string GetUnsupportedMessage() const;
  virtual void ApplyToCamera(const CameraOptionsType* src);
  virtual void PartiallyFillInCameraData(CameraOptionsType* target);

  static PersonTrackerAdapter* Instance();
  static void DestroyInstance();
  static v8::Local<v8::Promise> CreateRejectedPromise(std::string reason);
  static const char* event_names_[2];

 private:
  // helper methods
  PersonTrackerAdapter();
  void SetState(PersonTrackerState new_state) { state_ = new_state; }
  void SetErrorDescription(const char* description) {
    error_description_ = description;
  }
  std::string GetErrorDescription() { return error_description_; }
  std::wstring GetDataFilesPath();
  int32_t GetNextFrame();
  int8_t GetPixelSize(rs::format format);
  bool IsStreamCompatible(rs::core::stream_type stream,
      rs::format format, int width, int height, int frame_rate);
  void ProcessOutput(PT::PersonTrackingData* tracking_data);
  void UpdateInternalState(PT::PersonTrackingData* tracking_data);
  void StopCamera();
  void ConfigureCameraUsingSpecifiedData();
  void ResetInternalData();
  void ResetTracking();
  v8::Local<v8::Object> CreateResultObject();

  // If clean_all is true, clears everything, otherwise, only clear
  // internal_result_.
  void CleanResultObject(bool clean_all);
  bool IsRunning() {
    return (state_ != kStateReady &&
            state_ != kStateError &&
            state_ != kStatePaused);
  }
  bool IsErrored() { return state_ == kStateError; }

  void ExtractSkeletonData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractTrackData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractGestureData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractPersonFaceData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractExpressionData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractLandmarkData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  void ExtractPoseData(PT::PersonTrackingData::Person* p,
      PersonDataInternal* result);
  bool BuildupMaskInfoFromImage(RS::Image* image, bool is3D,
      MaskInfo* output);

  // Store frame data to frame_data_backup_ in the worker thread context.
  void BackupFrameDataInWorkerThread(
      rs::stream* stream,
      int* height,
      int* width,
      rs::format* format,
      double* timestamp,
      uint64_t* framenumber,
      const void** framedata);

  // Store the frame data to internal result which is shared with
  // main thread. Then empty frame_data_backup_.
  void ShareFrameDataToMainThread();
  void QueryModuleSupportedConfigs();
  void StoreImageForDelayedRelease(RS::Image* img);
  void ReleaseSegmentationImages();
  void ResetStateData();

  PersonTrackingConfig config_;
  CameraDelegate* ctx_;
  CameraDelegateDevice* device_;
  std::shared_ptr<rs::person_tracking::person_tracking_video_module_interface> pt_module_; //NOLINT
  rs::core::video_module_interface::actual_module_config actual_module_config_;
  std::mutex result_mutex_;
  PersonTrackResultInternal internal_result_;
  static PersonTrackerAdapter* tracker_instance_;
  bool device_started_;
  PersonTrackerState state_;
  PersonTrackerState internal_state_;
  bool has_internal_state_update_;
  bool camera_configured_;
  bool module_configured_;
  bool video_module_configured_;
  std::string error_description_;

  // This is updated by worker thread each time a new frame is got
  // from camera, and cleared when moved the data to internal result
  // which is shared between main thread.
  // The basic flow is:
  // 1. worker thread get a frame and store it to frame_data_backup_
  // in ProcessOutput().
  // 2. worker thread moved the data from frame_data_backup_ to
  // internal_result_ and clear frame_data_backup_ in ProcessOutput()
  // under mutex protection to avoid race condition.
  // 3. In OnSingleLoopFinished() in main thread, frame data is retrived
  // from internal_result_ and moved to frame_data_for_user_.
  // 4. User call getFrameData() to get data from frame_data_for_user_ in
  // main thread, and will not have race condition with step 3 as the two
  // operations are both in main thread.
  std::shared_ptr<FrameDataAssembler> frame_data_backup_;
  FrameData* frame_data_for_user_;
  std::shared_ptr<rs::core::correlated_sample_set> sample_set_;
  std::vector<std::shared_ptr<rs::core::video_module_interface::supported_module_config>> module_supported_configs_;  // NOLINT
  Nan::Persistent<v8::Object> js_this_;
  // Store the segmentation images here is to release them in a suitable time.
  std::vector<RS::Image*> segment_images_;
  static const PesonTrackerStateMap state_maps_[kStateCount];
  static const int32_t kDefaultRecognizePersonCount = 10;
  static const int32_t kDefaultRecognitionIDCount = 100;
};

#endif  // _WORKER_PERSON_TRACKING_ADAPTER_H_
