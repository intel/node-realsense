# Add your copyright and license header
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
{
  "targets": [
    {
      "target_name": "realsense_slam",
      "variables": {
        "rs_sdk_sysroot%": "",
        "widl_files": [
          "./idl/common/geometry.widl",
          "./idl/common/realsense-camera.widl",
          "./idl/slam/slam.widl",
        ],
        "gen_files": [
          "<(INTERMEDIATE_DIR)/gen/nan__motion_device_intrinsic.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__intrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__extrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__image.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__motion_sample.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__frame_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__point3d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__range.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__size2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__rect2d.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__occupancy_map_bounds.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__occupancy_map_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__tracking_result.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__event_info.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__tracking_event.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__instance.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__map_image.cpp",
        ],
      },
      "includes": [
        "./common/widl_nan_gen.gypi"
      ],
      "sources": [
        "addon.cpp",
        "common/extrinsics.cpp",
        "common/frame_data_assembler.cpp",
        "common/frame_data.cpp",
        "common/camera-options/camera_options_host.cpp",
        "common/camera-options/camera_options_host_instance.cpp",
        "common/geometry/point2d.cpp",
        "common/geometry/point3d.cpp",
        "common/geometry/range.cpp",
        "common/geometry/size2d.cpp",
        "common/geometry/rect2d.cpp",
        "common/image.cpp",
        "common/intrinsics.cpp",
        "common/motion_device_intrinsic.cpp",
        "common/motion_sample.cpp",
        "common/task/async_task.cpp",
        "common/task/async_task_runner.cpp",
        "common/task/async_task_runner_instance.cpp",
        "async_channel.cpp",
        "event_info.cpp",
        "instance.cpp",
        "instance_creator.cpp",
        "occupancy_map_data.cpp",
        "occupancy_map_bounds.cpp",
        "map_image.cpp",
        "slam_async_tasks.cpp",
        "slam_module.cpp",
        "slam_runner.cpp",
        "slam_handlers.cpp",
        "tracking_event.cpp",
        "tracking_result.cpp",
      ],
      "include_dirs": [
        ".",
        "<(INTERMEDIATE_DIR)",
        "common",
        "common/geometry",
        "<!(node -e \"require('nan')\")",
        "<@(rs_sdk_sysroot)/usr/include"
      ],
      "cflags!": [
        "-fno-exceptions"
      ],
      "cflags": [
        "-std=c++11"
      ],
      "cflags_cc!": [
        "-fno-exceptions",
        "-fno-rtti"
      ],
      "cflags_cc+": [
        "-frtti"
      ],
      "libraries": [
        "-L<@(rs_sdk_sysroot)/usr/lib",
        "-L<@(rs_sdk_sysroot)/usr/lib/x86_64-linux-gnu",
        "-lrealsense_projection",
        "-lrealsense_image",
        "-lrealsense_lrs_image",
        "-lrealsense_playback",
        "-lrealsense_record",
        "-lrealsense_log_utils",
        "-lpthread",
        "-lrealsense",
        "-lrealsense_slam",
        "-lSP_Core",
        "-ltracker"
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
