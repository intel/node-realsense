# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
{
  "targets": [
    {
      "target_name": "realsense_person_tracking",
      "variables": {
        "rs_sdk_sysroot%": "",
        "widl_files": [
          "idl/common/geometry.widl",
          "idl/common/realsense-camera.widl",
          "idl/person-tracking/person-tracking.widl",
        ],
        "gen_files": [
          "<(INTERMEDIATE_DIR)/gen/nan__bounding_box2d_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__expression_info_collection.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__expression_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__extrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__face_landmark.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__face_landmark_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__frame_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__image.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__intrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__gesture_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__lying_pose_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__lying_pose_recognition.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__mask_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__motion_device_intrinsic.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__motion_sample.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__orientation_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_face_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_recognition.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_recognizer_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_recognizer_data_with_status.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_registration_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_tracker.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_tracking.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__person_tracking_result.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point3d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point_combined_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__pointing_data2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__pointing_data3d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__pointing_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__pose_euler_angles.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__pose_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__range.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__rect2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__size2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__skeleton_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__skeleton_point_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__tracking_info.cpp",
        ]
      },
      "includes": [
        "./common/widl_nan_gen.gypi"
      ],
      "sources": [
        "addon.cpp",
        "bounding_box2d_info.cpp",
        "common/camera-delegate/camera_delegate.cpp",
        "common/camera-delegate/camera_delegate_instance.cpp",
        "common/camera-options/camera_options_host.cpp",
        "common/camera-options/camera_options_host_instance.cpp",
        "common/extrinsics.cpp",
        "common/frame_data_assembler.cpp",
        "common/frame_data.cpp",
        "common/geometry/point2d.cpp",
        "common/geometry/point3d.cpp",
        "common/geometry/range.cpp",
        "common/geometry/rect2d.cpp",
        "common/geometry/size2d.cpp",
        "common/image.cpp",
        "common/intrinsics.cpp",
        "common/motion_sample.cpp",
        "common/motion_device_intrinsic.cpp",
        "common/task/async_task.cpp",
        "common/task/async_task_runner.cpp",
        "common/task/async_task_runner_instance.cpp",
        "expression_info_collection.cpp",
        "expression_info.cpp",
        "face_landmark.cpp",
        "face_landmark_info.cpp",
        "gesture_info.cpp",
        "lying_pose_info.cpp",
        "lying_pose_recognition.cpp",
        "mask_info.cpp",
        "orientation_info.cpp",
        "person_face_info.cpp",
        "person_info.cpp",
        "person_recognition.cpp",
        "person_recognizer_data.cpp",
        "person_recognizer_data_with_status.cpp",
        "person_registration_data.cpp",
        "person_tracker.cpp",
        "person_tracking.cpp",
        "person_tracking_result.cpp",
        "point_combined_info.cpp",
        "pointing_data2d.cpp",
        "pointing_data3d.cpp",
        "pointing_info.cpp",
        "pose_euler_angles.cpp",
        "pose_info.cpp",
        "skeleton_info.cpp",
        "skeleton_point_info.cpp",
        "tracking_info.cpp",
        "worker/person_data_internal.cpp",
        "worker/person_tracker_adapter.cpp",
        "worker/person_tracker_creator.cpp",
        "worker/person_tracker_runner_proxy.cpp",
        "worker/person_tracking_config.cpp",
        "worker/person_tracking_result_internal.cpp",
        "worker/person_tracking_tasks.cpp",
        "worker/utils.cpp"
      ],
      "include_dirs": [
        ".",
        "<(INTERMEDIATE_DIR)",
        "./common",
        "./common/geometry",
        "<!(node -e \"require('nan')\")",
        "<@(rs_sdk_sysroot)/usr/include",
        "<@(rs_sdk_sysroot)/usr/include/librealsense/pt/"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags": [
        "-std=c++11"
      ],
      "cflags_cc!": [
        "-fno-exceptions"
      ],
      "libraries": [
        "-L<@(rs_sdk_sysroot)/usr/lib",
        "-L<@(rs_sdk_sysroot)/usr/lib/x86_64-linux-gnu",
        "-lpthread",
        "-lrealsense",
        "-lrealsense_image",
        "-lrealsense_persontracking",
        "-lrealsense_playback",
        "-lrealsense_projection"
      ],
      "xcode_settings": {
        "OTHER_CFLAGS": [
          "-std=c++11"
        ]
      },
      "conditions": [
        [
          "OS!=\"win\"",
          {
            "cflags+": [
              "-std=c++11"
            ],
            "cflags_c+": [
              "-std=c++11"
            ],
            "cflags_cc+": [
              "-std=c++11"
            ]
          }
        ],
        [
          "OS==\"mac\"",
          {
            "xcode_settings": {
              "OTHER_CPLUSPLUSFLAGS": [
                "-std=c++11",
                "-stdlib=libc++"
              ],
              "OTHER_LDFLAGS": [
                "-stdlib=libc++"
              ],
              "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
              "MACOSX_DEPLOYMENT_TARGET": "10.8"
            }
          }
        ]
      ]
    }
  ]
}
