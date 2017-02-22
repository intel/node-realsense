// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _OBJECT_RECOGNITION_RUNNER_H_
#define _OBJECT_RECOGNITION_RUNNER_H_

#include <string>
#include <vector>

#include "common/type/v8_alias.h"
#include "common/camera-options/camera_options_io.h"
#include "gen/camera_options.h"
#include "gen/object_recognition_options.h"
#include "instance_state_changer.h"

class ObjectRecognitionRunner:
    public InstanceStateChanger,
    public CameraOptionsIO {
 private:
  ObjectRecognitionRunner();

 public:
  ~ObjectRecognitionRunner();

  void ExportOptions(DictionaryObjectRecognitionOptions* options);
  bool ImportOptions(const DictionaryObjectRecognitionOptions& options);

  bool Init();
  bool Run();
  bool IsLastFrameProcessed() const;
  bool RetrieveCameraFrame();
  bool ProcessCameraFrame();
  void GetCameraFrame();
  std::string GetEventName() const;
  bool IsResultReady() const;
  v8_value_t GetResult();
  v8_value_t GetFrameData();
  void Stop();
  void Reset();
  void Cleanup();

 public:
  virtual bool IsSupported(const CameraOptionsType* src) const;

  virtual std::string GetUnsupportedMessage() const;

  virtual void ApplyToCamera(const CameraOptionsType* src);

  virtual void PartiallyFillInCameraData(CameraOptionsType* target);


  bool DumpRecognizableObjectNames(std::vector<std::string>* target);

  static ObjectRecognitionRunner* GetRunnerInstance();
  static void DestroyRunnerInstance();

 public:
  virtual std::string GetInstanceState() const;
  virtual void SetInstanceState(const std::string& state);

 private:
  class ObjectRecognitionRunnerD * d_;
};

#endif  // _OBJECT_RECOGNITION_RUNNER_H_
