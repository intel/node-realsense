# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# This file is meant to be included into a target to provide a rule
# to invoke src/common/sctipts/process_idl.js.
#
# To use this, create a gyp target with the following form:
# {
#   "target_name": "my_addon",
#   "variables": {
#     "widl_files": [
#       "bar.widl",
#       "foo.widl",
#     ],
#     "gen_files": [
#       "<(INTERMEDIATE_DIR)/gen/bar.cpp",
#       "<(INTERMEDIATE_DIR)/gen/foo.cpp",
#     ],
#    },
#   "includes": [
#     "path/to/this/gypi/file"
#   ],
#   "include_dirs": [
#     "<(INTERMEDIATE_DIR)",
#   ],
# }

{
  "copies": [
    {
      'destination': '<(INTERMEDIATE_DIR)',
      'files': [
        '<(DEPTH)/common/scripts/process_idl.js',
      ],
    },
  ],
  "actions": [
    {
      "action_name": "widl_gen",
      "variables": {
        "widl_gen_script": "<(INTERMEDIATE_DIR)/process_idl.js",
      },
      "inputs": [
        "<(widl_gen_script)",
        "<@(widl_files)",
      ],
      "outputs": [
        "<@(gen_files)",
      ],
      "action": [
        "node",
        "<(widl_gen_script)",
        "<(INTERMEDIATE_DIR)/gen",
        "<@(widl_files)",
      ],
      'process_outputs_as_sources': 1,
      "message": "Generating NAN C++ code from WIDL files",
    },
  ],
}
