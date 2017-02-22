#!/bin/bash
#
# Copyright (c) 2017 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#

set -e

MOUDLES="
object-recognition
person-tracking
slam
"

pushd $(dirname $0) > /dev/null

NPM_CMDS="install;test"

# Execute for each module
function build_module {
  module_name=$1

  echo -e "\n\n---------Building ${module_name}----------"

  cd src/${module_name}
  # Cleanup environment
  rm -rf node_modules/ build/ *.tar.gz

  error="0"
  IFS=';'
  for cmd in ${NPM_CMDS}; do
    echo -e "\n\nRun npm $cmd"
    eval "npm $cmd" || error="$cmd"
  done

  if [ "$error" = "0" ]; then
    msg="${module_name}: SUCCEED"
  else
    msg="${module_name}: FAILED on $error"
  fi

  echo -e "\n\n ${msg}"

  cd ../..
}

cd ..
for i in ${MOUDLES}; do
  build_module $i
done

popd > /dev/null
