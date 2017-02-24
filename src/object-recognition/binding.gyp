# Add your copyright and license header
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
{
  "targets": [
    {
      "target_name": "realsense_object_recognition",
      "variables": {
        "rs_sdk_sysroot%": "",
        "widl_files": [
          "./idl/common/geometry.widl",
          "./idl/common/realsense-camera.widl",
          "./idl/object-recognition/object-recognition.widl",
        ],
        "gen_files": [
          "<(INTERMEDIATE_DIR)/gen/nan__extrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__frame_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__image.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__object_recognizer.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__intrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__localization_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__tracking_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__motion_device_intrinsic.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__motion_sample.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point3d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__range.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__recognition_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__rect2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__size2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/thread-event-helper.cpp",
        ],
      },
      "includes": [
        "./common/widl_nan_gen.gypi"
      ],
      "sources": [
        "addon.cpp",
        "common/camera-options/camera_options_host.cpp",
        "common/camera-options/camera_options_host_instance.cpp",
        "common/extrinsics.cpp",
        "common/frame_data.cpp",
        "common/geometry/point_base.cpp",
        "common/geometry/point2d.cpp",
        "common/geometry/point3d.cpp",
        "common/geometry/range.cpp",
        "common/geometry/rect2d.cpp",
        "common/geometry/size2d.cpp",
        "common/image.cpp",
        "common/intrinsics.cpp",
        "common/motion_device_intrinsic.cpp",
        "common/motion_sample.cpp",
        "common/frame_data_assembler.cpp",
        "common/task/async_task.cpp",
        "common/task/async_task_runner.cpp",
        "object_recognition_label_name_translator.cpp",
        "object_recognition_result_assembler.cpp",
        "common/task/async_task_runner_instance.cpp",
        "object_recognition_runner.cpp",
        "object_recognition_tasks.cpp",
        "object_recognizer.cpp",
        "instance_creator.cpp",
        "camera_runner.cpp",
        "localization_info.cpp",
        "tracking_info.cpp",
        "recognition_info.cpp",
      ],
      "include_dirs": [
        ".",
        "<(INTERMEDIATE_DIR)",
        "common",
        "common/geometry",
        "<!(node -e \"require('nan')\")",
        "<@(rs_sdk_sysroot)/usr/include",
        "<@(rs_sdk_sysroot)/usr/include/librealsense/object_recognition",
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
        "-lrealsense_object_recognition",
        "-lrealsense_image",
        "-lrealsense_playback",
        "-lrealsense_projection",
        "-lrealsense",
        "-L<@(rs_sdk_sysroot)/usr/lib/x86_64-linux-gnu",
        "-L<@(rs_sdk_sysroot)/usr/lib"
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
