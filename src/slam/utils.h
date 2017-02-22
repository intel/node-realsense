// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#ifndef _UTILS_H_
#define _UTILS_H_

#include <execinfo.h>
#include <librealsense/rs.hpp>
#include <librealsense/slam/slam.h>
#include <librealsense/slam/slam_utils.h>
#include <rs/core/status.h>
#include <rs/core/types.h>

#include <iostream>
#include <memory>
#include <string>

#include "default_values.h"
#include "gen/promise-helper.h"

/**
 * 1. kIdle --(Init)--> ready --(Start)--> tracking
 * 2. tracking --(Stop)--> ready --(destroy)--> kIdle
 */
enum class SlamState {
  kIdle = 0,
  kReady = 1,
  kTracking = 2,
  kPaused = 3
};

enum class SlamEvent {
  kTracking = 0,
  kError = 1
};

namespace utils {
class Status {
 private:
  int id_;
  std::string message_;

 public:
  explicit Status(int status_id) : id_(status_id) {}

  Status() : id_(rs::core::status_no_error) {}

  Status(int status_id, const std::string &error_message)
      : id_(status_id), message_(error_message) {}

  int id() { return id_; }

  void set_id(int id) { id_ = id; }

  void set_message(const std::string &message) { message_ = message; }

  std::string message() {
    if (!message_.empty()) return message_;

    switch (id_) {
      case rs::core::status_no_error:
        return "succeed";

      // Errors.
      case rs::core::status_feature_unsupported:
        return "unsupported feature";
      case rs::core::status_param_unsupported:
        return "unsupported parameter(s)";
      case rs::core::status_item_unavailable:
        return "item not found/not available";

      case rs::core::status_handle_invalid:
        return "invalid session, algorithm instance, or pointer";
      case rs::core::status_alloc_failed:
        return "memory allocation failure";

      case rs::core::status_device_failed:
        return "device failed due to malfunctioning";
      case rs::core::status_device_lost:
        return "device failed due to unplug or unavailability";

      case rs::core::status_exec_aborted:
        return "execution aborted due to errors in upstream components";
      case rs::core::status_exec_inprogress:
        return "asynchronous operation is in execution";
      case rs::core::status_exec_timeout:
        return "operation time out";

      case rs::core::status_file_write_failed:
        return "failure in open file in WRITE mode";
      case rs::core::status_file_read_failed:
        return "failure in open file in READ mode";
      case rs::core::status_file_close_failed:
        return "failure in close a file handle";
      case rs::core::status_file_open_failed:
        return "failure in open a file handle";

      case rs::core::status_data_unavailable:
        return "data not available for MW model or processing";

      case rs::core::status_data_not_initialized:
        return "data failed to initialize";
      case rs::core::status_init_failed:
        return "module failure during initialization";

      case rs::core::status_match_not_found:
        return "matching frame not found";

      // Warnings.
      case rs::core::status_time_gap:
        return "time gap in time stamps";
      case rs::core::status_param_inplace:
        return "the same parameters already defined";
      case rs::core::status_data_not_changed:
        return "data not changed (no new data available)";
      case rs::core::status_process_failed:
        return "module failure during processing";
      case rs::core::status_value_out_of_range:
        return "data value(s) out of range";
      case rs::core::status_data_pending:
        return "not all data was copied, more data is available for fetching";
      default:
        return "slam addon error";
    }
  }
};

inline void log(const std::string &error_message) {
  if (DEBUG)
    std::cout << error_message << std::endl;
}

inline void OnSlamError(utils::Status e) {
  // TODO(Donna): need to emit error to JS.
  if (DEBUG)
    utils::log(SLAM_ERROR_PREFIX + e.message());
}

inline void OnSlamError(const std::string &error_message) {
  if (DEBUG)
    utils::log(SLAM_ERROR_PREFIX + error_message);
}

inline std::string AccuracyToString(
    rs::slam::tracking_accuracy accuracy) {
  switch (accuracy) {
    case rs::slam::tracking_accuracy::failed:
      return "failed";
    case rs::slam::tracking_accuracy::low:
      return "low";
    case rs::slam::tracking_accuracy::medium:
      return "med";
    case rs::slam::tracking_accuracy::high:
      return "high";
    default:
      return "";
  }
}

inline std::string SlamStateToString(SlamState state) {
  switch (state) {
    case SlamState::kReady: return "ready";
    case SlamState::kTracking: return "tracking";
    case SlamState::kPaused: return "paused";
                             // IDLE state should not be exposed to user.
    default: return "";
  }
}

inline std::string SlamEventToString(SlamEvent event_name) {
  switch (event_name) {
    case SlamEvent::kError: return "error";
    case SlamEvent::kTracking: return "tracking";
    default: return "";
  }
}

inline rs::core::motion_type RsToSdkMotionType(
    rs_event_source motionSource) {
  rs::core::motion_type motion_type = rs::core::motion_type::max;

  switch (motionSource) {
    case RS_EVENT_IMU_ACCEL:
      motion_type = rs::core::motion_type::accel;
      break;
    case RS_EVENT_IMU_GYRO:
      motion_type = rs::core::motion_type::gyro;
      break;
    case RS_EVENT_IMU_DEPTH_CAM:
    case RS_EVENT_IMU_MOTION_CAM:
    case RS_EVENT_G0_SYNC:
    case RS_EVENT_G1_SYNC:
    case RS_EVENT_G2_SYNC:
    case RS_EVENT_SOURCE_COUNT:
      break;
  }
  return motion_type;
}

inline void ClearStreams(
    rs::core::video_module_interface::supported_module_config* config) {
  for (int i = 0; i < static_cast<int>(rs::core::stream_type::max); ++i) {
    (*config)[rs::core::stream_type(i)].is_enabled = false;
  }

  for (int i = static_cast<int>(rs::core::motion_type::accel);
      i < static_cast<int>(rs::core::motion_type::max); ++i) {
    (*config)[rs::core::motion_type(i)].is_enabled = false;
  }
}

inline rs::source GetSourceType(
    rs::core::video_module_interface::supported_module_config* config) {
  rs::source active_sources = static_cast<rs::source>(0);
  for (int i = 0; i < static_cast<int>(rs::core::stream_type::max); ++i) {
    if ((*config)[rs::core::stream_type(i)].is_enabled) {
      active_sources = rs::source::video;
      break;
    }
  }

  for (int i = static_cast<int>(rs::core::motion_type::accel);
      i < static_cast<int>(rs::core::motion_type::max); ++i) {
    if ((*config)[rs::core::motion_type(i)].is_enabled) {
      active_sources = (active_sources == rs::source::video) ?
                       rs::source::all_sources : rs::source::motion_data;
      break;
    }
  }
  return active_sources;
}

inline bool CheckMotionSensorCapabilityIfRequired(
    rs::device* device, rs::source source) {
  if (source == rs::source::all_sources ||
      source == rs::source::motion_data)
    return device->supports(rs::capabilities::motion_events);
  return true;
}

inline bool IsMotionStreamRequested(rs::source source) {
  return (source == rs::source::all_sources ||
          source == rs::source::motion_data);
}

inline void PrintBackTrace(void) {
    int j, nptrs;
    void* buffer[BT_BUF_SIZE];
    char** strings;

    nptrs = backtrace(buffer, BT_BUF_SIZE);
    printf("backtrace() returned %d addresses\n", nptrs);

    // The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
    // would produce similar output to the following:

    strings = backtrace_symbols(buffer, nptrs);
    if (strings == NULL) {
        perror("backtrace_symbols");
        exit(EXIT_FAILURE);
    }
    for (j = 0; j < nptrs; j++)
        printf("%s\n", strings[j]);

    free(strings);
}
}  // namespace utils

#endif  // _UTILS_H_
