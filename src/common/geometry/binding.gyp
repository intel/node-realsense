# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
{
  "targets": [
    {
      "target_name": "geometry",
      "sources": [
        "addon.cpp",
        "gen/nan__point2d.cpp",
        "gen/nan__point3d.cpp",
        "gen/nan__range.cpp",
        "gen/nan__size2d.cpp",
        "gen/nan__rect2d.cpp",
        "point_base.cpp",
        "point2d.cpp",
        "point3d.cpp",
        "range.cpp",
        "size2d.cpp",
        "rect2d.cpp",
      ],
      "include_dirs": [
        "<!(node -e \"require('nan')\")",
        "../..",
        "."
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
