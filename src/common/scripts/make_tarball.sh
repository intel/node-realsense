#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# This script is to make a tarball of current npm directory.
#
# npm pack is not suitable because it will bundled all files

module_name=$1
if [ ${module_name} = "" ]; then
  echo "Moudle name is not specified."
  exit module_name
fi

version=$(node -e "console.log(require('./package.json').version);")

rm ${module_name}-${version}.tar ${module_name}-${version}.tar.gz

tar cf ${module_name}-${version}.tar \
    --exclude-vcs --exclude=node_modules \
    --exclude=gen --exclude=build \
    --exclude=common/tests/data \
    --dereference .

gzip ${module_name}-${version}.tar
