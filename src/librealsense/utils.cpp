// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

#include <utils.h>

rs::stream GetRSStreamFromString(const std::string& stream) {
  if (stream == "depth")
    return rs::stream::depth;
  else if (stream == "color")
    return rs::stream::color;
  else if (stream == "infrared")
    return rs::stream::infrared;
  else if (stream == "infrared2")
    return rs::stream::infrared2;
  else if (stream == "points")
    return rs::stream::points;
  else if (stream == "rectified_color")
    return rs::stream::rectified_color;
  else if (stream == "color_aligned_to_depth")
    return rs::stream::color_aligned_to_depth;
  else if (stream == "infrared2_aligned_to_depth")
    return rs::stream::infrared2_aligned_to_depth;
  else if (stream == "depth_aligned_to_color")
    return rs::stream::depth_aligned_to_color;
  else if (stream == "depth_aligned_to_rectified_color")
    return rs::stream::depth_aligned_to_rectified_color;
  else if (stream == "depth_aligned_to_infrared2")
    return rs::stream::depth_aligned_to_infrared2;
  else
    return rs::stream::depth;
}

std::string GetStringFromRSStream(const rs::stream stream) {
  switch (stream) {
    case rs::stream::depth:
      return "depth";
    case rs::stream::color:
      return "color";
    case rs::stream::infrared:
      return "infrared";
    case rs::stream::infrared2:
      return "infrared2";
    case rs::stream::points:
      return "points";
    case rs::stream::rectified_color:
      return "rectified_color";
    case rs::stream::color_aligned_to_depth:
      return "color_aligned_to_depth";
    case rs::stream::infrared2_aligned_to_depth:
      return "infrared2_aligned_to_depth";
    case rs::stream::depth_aligned_to_color:
      return "depth_aligned_to_color";
    case rs::stream::depth_aligned_to_infrared2:
      return "depth_aligned_to_infrared2";
    case rs::stream::depth_aligned_to_rectified_color:
      return "depth_aligned_to_rectified_color";
    default:
      return std::string();
  }
}

rs::option GetRSOptionFromString(const std::string& option) {
  if (option == "color_backlight_compensation")
    return rs::option::color_backlight_compensation;
  else if (option == "color_brightness")
    return rs::option::color_brightness;
  else if (option == "color_contrast")
    return rs::option::color_contrast;
  else if (option == "color_exposure")
    return rs::option::color_exposure;
  else if (option == "color_gain")
    return rs::option::color_gain;
  else if (option == "color_gamma")
    return rs::option::color_gamma;
  else if (option == "color_hue")
    return rs::option::color_hue;
  else if (option == "color_saturation")
    return rs::option::color_saturation;
  else if (option == "color_sharpness")
    return rs::option::color_sharpness;
  else if (option == "color_white_balance")
    return rs::option::color_white_balance;
  else if (option == "color_enable_auto_exposure")
    return rs::option::color_enable_auto_exposure;
  else if (option == "color_enable_auto_white_balance")
    return rs::option::color_enable_auto_white_balance;
  else if (option == "f200_laser_power")
    return rs::option::f200_laser_power;
  else if (option == "f200_accuracy")
    return rs::option::f200_accuracy;
  else if (option == "f200_motion_range")
    return rs::option::f200_motion_range;
  else if (option == "f200_filter_option")
    return rs::option::f200_filter_option;
  else if (option == "f200_confidence_threshold")
    return rs::option::f200_confidence_threshold;
  else if (option == "f200_dynamic_fps")
    return rs::option::f200_dynamic_fps;
  else if (option == "sr300_auto_range_enable_motion_versus_range")
    return rs::option::sr300_auto_range_enable_motion_versus_range;
  else if (option == "sr300_auto_range_enable_laser")
    return rs::option::sr300_auto_range_enable_laser;
  else if (option == "sr300_auto_range_min_motion_versus_range")
    return rs::option::sr300_auto_range_min_motion_versus_range;
  else if (option == "sr300_auto_range_max_motion_versus_range")
    return rs::option::sr300_auto_range_max_motion_versus_range;
  else if (option == "sr300_auto_range_start_motion_versus_range")
    return rs::option::sr300_auto_range_start_motion_versus_range;
  else if (option == "sr300_auto_range_min_laser")
    return rs::option::sr300_auto_range_min_laser;
  else if (option == "sr300_auto_range_max_laser")
    return rs::option::sr300_auto_range_max_laser;
  else if (option == "sr300_auto_range_start_laser")
    return rs::option::sr300_auto_range_start_laser;
  else if (option == "sr300_auto_range_upper_threshold")
    return rs::option::sr300_auto_range_upper_threshold;
  else if (option == "sr300_auto_range_lower_threshold")
    return rs::option::sr300_auto_range_lower_threshold;
  else if (option == "r200_lr_auto_exposure_enabled")
    return rs::option::r200_lr_auto_exposure_enabled;
  else if (option == "r200_lr_gain")
    return rs::option::r200_lr_gain;
  else if (option == "r200_lr_exposure")
    return rs::option::r200_lr_exposure;
  else if (option == "r200_emitter_enabled")
    return rs::option::r200_emitter_enabled;
  else if (option == "r200_depth_units")
    return rs::option::r200_depth_units;
  else if (option == "r200_depth_clamp_min")
    return rs::option::r200_depth_clamp_min;
  else if (option == "r200_depth_clamp_max")
    return rs::option::r200_depth_clamp_max;
  else if (option == "r200_disparity_multiplier")
    return rs::option::r200_disparity_multiplier;
  else if (option == "r200_disparity_shift")
    return rs::option::r200_disparity_shift;
  else if (option == "r200_auto_exposure_mean_intensity_set_point")
    return rs::option::r200_auto_exposure_mean_intensity_set_point;
  else if (option == "r200_auto_exposure_bright_ratio_set_point")
    return rs::option::r200_auto_exposure_bright_ratio_set_point;
  else if (option == "r200_auto_exposure_kp_gain")
    return rs::option::r200_auto_exposure_kp_gain;
  else if (option == "r200_auto_exposure_kp_exposure")
    return rs::option::r200_auto_exposure_kp_exposure;
  else if (option == "r200_auto_exposure_kp_dark_threshold")
    return rs::option::r200_auto_exposure_kp_dark_threshold;
  else if (option == "r200_auto_exposure_top_edge")
    return rs::option::r200_auto_exposure_top_edge;
  else if (option == "r200_auto_exposure_bottom_edge")
    return rs::option::r200_auto_exposure_bottom_edge;
  else if (option == "r200_auto_exposure_left_edge")
    return rs::option::r200_auto_exposure_left_edge;
  else if (option == "r200_auto_exposure_right_edge")
    return rs::option::r200_auto_exposure_right_edge;
  else if (option == "r200_depth_control_estimate_median_decrement")
    return rs::option::r200_depth_control_estimate_median_decrement;
  else if (option == "r200_depth_control_estimate_median_increment")
    return rs::option::r200_depth_control_estimate_median_increment;
  else if (option == "r200_depth_control_median_threshold")
    return rs::option::r200_depth_control_median_threshold;
  else if (option == "r200_depth_control_score_minimum_threshold")
    return rs::option::r200_depth_control_score_minimum_threshold;
  else if (option == "r200_depth_control_score_maximum_threshold")
    return rs::option::r200_depth_control_score_maximum_threshold;
  else if (option == "r200_depth_control_texture_count_threshold")
    return rs::option::r200_depth_control_texture_count_threshold;
  else if (option == "r200_depth_control_texture_difference_threshold")
    return rs::option::r200_depth_control_texture_difference_threshold;
  else if (option == "r200_depth_control_second_peak_threshold")
    return rs::option::r200_depth_control_second_peak_threshold;
  else if (option == "r200_depth_control_neighbor_threshold")
    return rs::option::r200_depth_control_neighbor_threshold;
  else if (option == "r200_depth_control_lr_threshold")
    return rs::option::r200_depth_control_lr_threshold;
  else
    return rs::option::color_backlight_compensation;
}


std::string GetStringFromRSFormat(const rs::format format) {
  switch (format) {
    case rs::format::any:
      return "any";
    case rs::format::z16:
      return "z16";
    case rs::format::disparity16:
      return "disparity16";
    case rs::format::xyz32f:
      return "xyz32f";
    case rs::format::yuyv:
      return "yuyv";
    case rs::format::rgb8:
      return "rgb8";
    case rs::format::bgr8:
      return "bgr8";
    case rs::format::rgba8:
      return "rgba8";
    case rs::format::bgra8:
      return "bgra8";
    case rs::format::y8:
      return "y8";
    case rs::format::y16:
      return "y16";
    case rs::format::raw10:
      return "raw10";
    default:
      return std::string();
  }
}

rs::format GetRSFormatFromString(const std::string& format) {
  if (format == "z16")
    return rs::format::z16;
  else if (format == "disparity16")
    return rs::format::disparity16;
  else if (format == "xyz32f")
    return rs::format::xyz32f;
  else if (format == "yuyv")
    return rs::format::yuyv;
  else if (format == "rgb8")
    return rs::format::rgb8;
  else if (format == "bgr8")
    return rs::format::bgr8;
  else if (format == "rgba8")
    return rs::format::rgba8;
  else if (format == "bgra8")
    return rs::format::bgra8;
  else if (format == "y8")
    return rs::format::y8;
  else if (format == "y16")
    return rs::format::y16;
  else if (format == "raw10")
    return rs::format::raw10;
  else
    return rs::format::any;
}

rs::camera_info GetCameraInfoFromString(const std::string& info) {
  if (info == "device_name")
    return rs::camera_info::device_name;
  else if (info == "serial_number")
    return rs::camera_info::serial_number;
  else if (info == "camera_firmware_version")
    return rs::camera_info::camera_firmware_version;
  else if (info == "adapter_board_firmware_version")
    return rs::camera_info::adapter_board_firmware_version;
  else if (info == "motion_module_firmware_version")
    return rs::camera_info::motion_module_firmware_version;
  else if (info == "camera_type")
    return rs::camera_info::camera_type;
  else if (info == "oem_id")
    return rs::camera_info::oem_id;
  else if (info == "isp_fw_version")
    return rs::camera_info::isp_fw_version;
  else if (info == "content_version")
    return rs::camera_info::content_version;
  else if (info == "module_version")
    return rs::camera_info::module_version;
  else if (info == "imager_model_number")
    return rs::camera_info::imager_model_number;
  else if (info == "build_date")
    return rs::camera_info::build_date;
  else if (info == "calibration_date")
    return rs::camera_info::calibration_date;
  else if (info == "program_date")
    return rs::camera_info::program_date;
  else if (info == "focus_alignment_date")
    return rs::camera_info::focus_alignment_date;
  else if (info == "emitter_type")
    return rs::camera_info::emitter_type;
  else if (info == "focus_value")
    return rs::camera_info::focus_value;
  else if (info == "lens_type")
    return rs::camera_info::lens_type;
  else if (info == "third_lens_type")
    return rs::camera_info::third_lens_type;
  else if (info == "lens_coating_type")
    return rs::camera_info::lens_coating_type;
  else if (info == "third_lens_coating_type")
    return rs::camera_info::third_lens_coating_type;
  else if (info == "lens_nominal_baseline")
    return rs::camera_info::lens_nominal_baseline;
  else if (info == "third_lens_nominal_baseline")
    return rs::camera_info::third_lens_nominal_baseline;
  else
    return rs::camera_info::device_name;
}
