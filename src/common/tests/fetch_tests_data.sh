#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# This script is to download recorded data for automation testing.
#

pushd $(dirname $0) > /dev/null

cd ../; wget -r -nH --cut-dirs=2 --no-parent --reject="index.html*" $1

popd > /dev/null
