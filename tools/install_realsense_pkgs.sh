#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# This script is to install RealSense packages.
#
# TODOs: 
# * Internal link suppose to be removed when public available.
#

if [ ! $(id -u) = "0" ]; then
  echo "You need run this script with root or sudo"
  exit 1
fi

sudo apt-key adv --keyserver keys.gnupg.net --recv-key D6FB2970
echo 'deb http://realsense-alm-public.s3.amazonaws.com/apt-repo xenial main' | sudo tee /etc/apt/sources.list.d/realsense-public.list
apt-get update
apt-get install -y --allow-unauthenticated librealsense-all-dev=0.0.1~rc0-0ubuntu2~16.gbp3bd28d
# TODO: Remove boost dependency in futhure
apt-get install -y libboost1.58-dev
