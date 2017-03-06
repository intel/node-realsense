// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//

#include "slam_handlers.h"

#include "common/task/async_task_runner_instance.h"
#include "output_holder.h"
#include "slam_async_tasks.h"

OutputHolder* SlamEventHandler::MoveResult() {
  std::lock_guard<std::mutex> lock(mutex_);
  auto copy = output_holder_;
  output_holder_ = nullptr;
  return copy;
}

void SlamEventHandler::module_output_ready(
    rs::core::video_module_interface* sender,
    rs::core::correlated_sample_set* sample) {
  // post async to worker
  rs::slam::slam* slam = dynamic_cast<rs::slam::slam*>(sender);

  // 1. Camera pose update
  rs::slam::PoseMatrix4f pose;
  slam->get_camera_pose(pose);
  auto accuracy = slam->get_tracking_accuracy();

  // 2. Notice the main thread to emit "tracking" event.
  auto payload = new TrackingEventPayload(SlamRunner::GetSlamRunner(),
      new TrackedResultInternal(accuracy, pose));

  AsyncTaskRunnerInstance::GetInstance()->PostTask(
      new TrackingEventTask(),
      payload,
      "{{TRACKING_EVENT MESSAGE}}");

  // 3. Copy result to holder. Output_holer_ will be updated every frame.
  auto frame_data_assembler = std::make_shared<FrameDataAssembler>();
  frame_data_assembler->SetData(
      sample->images[static_cast<int>(rs::core::stream_type::fisheye)]);
  // Depth image is not always available.
  auto depth_index = static_cast<int>(rs::core::stream_type::depth);
  if (sample->images[depth_index]) {
    frame_data_assembler->SetData(sample->images[depth_index]);
  }
  auto motion_type = rs::core::motion_type::gyro;
  auto gyro_sample = sample->motion_samples[static_cast<int>(motion_type)];
  // We need to set type back as type value got from sampleset is 0.
  // This is a native bug.
  gyro_sample.type = motion_type;
  frame_data_assembler->SetData(gyro_sample);

  motion_type = rs::core::motion_type::accel;
  auto accel_sample = sample->motion_samples[static_cast<int>(motion_type)];
  accel_sample.type = motion_type;
  frame_data_assembler->SetData(accel_sample);

  std::lock_guard<std::mutex> lock(mutex_);
  if (output_holder_ == nullptr) output_holder_ = new OutputHolder;

  output_holder_->SetData(accuracy, pose, frame_data_assembler);
}
