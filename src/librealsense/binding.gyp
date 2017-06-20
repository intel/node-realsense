# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
{
  "targets": [
    {
      "target_name": "node_librealsense",
      "variables": {
        "gen_files": [
          "<(INTERMEDIATE_DIR)/gen/nan__context.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__device.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__frame_data.cpp",
          "<(INTERMEDIATE_DIR)/gen/nan__rs_extrinsics.cpp",
          "<(INTERMEDIATE_DIR)/gen/thread-event-helper.cpp",
        ],
        "widl_files": [
          "idl/node-librealsense.widl",
        ],
      },
      "includes": [
        "./common/widl_nan_gen.gypi"
      ],
      "sources": [
        "addon.cpp",
        "common/extrinsics.cpp",
        "common/task/async_task.cpp",
        "common/task/async_task_runner.cpp",
        "common/task/async_task_runner_instance.cpp",
        "context.cpp",
        "context_runner.cpp",
        "device.cpp",
        "device_runner.cpp",
        "frame_data.cpp",
        "rs_extrinsics.cpp",
        "rs_task.cpp",
        "utils.cpp",
      ],
      "include_dirs": [
        ".",
        "common",
        "<(INTERMEDIATE_DIR)",
        "<!(node -e \"require('nan')\")",
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
        "-lrealsense",
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
