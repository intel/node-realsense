#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#
# This script is to remove RealSense Beta3 files and packages.
#

if [ ! $(id -u) = "0" ]; then
  echo "You need run this script with root or sudo"
  exit 1
fi

echo "Clean up for librealsense"
rm -rf /usr/lib/x86_64-linux-gnu/librealsense.so
rm -rf /usr/include/librealsense
rm -rf /usr/bin/c-tutorial-*
rm -rf /usr/bin/cpp-*

echo "Clean up for realsense-sdk"
rm -rf /usr/include/rs_*.h
rm -rf /usr/include/rs/
rm -rf /usr/bin/rs_*
rm -rf /usr/lib/librealsense_*so*

echo "Clean up for librealsense-persontracking"
rm -rf /usr/lib/librealsense_persontracking.so
rm -rf /usr/share/librealsense/pt/
rm -rf /usr/include/librealsense/pt/

echo "Clean up for librealsense-objectrecognition"
rm -rf /usr/lib/librealsense_object_recognition.so
rm -rf /usr/lib/libcaffe.so.1.0.0-rc3
rm -rf /usr/lib/libcaffe.so
rm -rf /usr/share/librealsense/object_recognition/
rm -rf /usr/include/librealsense/object_recognition/

echo "Clean up for librealsense-slam"
rm -rf /usr/lib/libSP_Core.so*
rm -rf /usr/lib/librealsense_slam.so*
rm -rf /usr/lib/libtracker.so*
rm -rf /usr/include/librealsense/slam/

echo "Clean up for realsense_samples"
rm -rf /usr/share/librealsense/samples
rm -rf /usr/bin/sample_or_*
rm -rf /usr/bin/sample_pt_*
rm -rf /usr/bin/sample_slam_*
rm -rf /usr/bin/libremote_display.a

echo "Clean up for libopencv3"
rm -rf /usr/bin/open_*
rm -rf /usr/include/opencv/
rm -rf /usr/include/opencv2/
rm -rf /usr/lib/pkgconfig/opencv.pc
rm -rf /usr/lib/libopencv_*.so*
rm -rf /usr/lib/python2.7/dist-packages/cv2.so
rm -rf /usr/share/OpenCV/

echo "Clean up for beignet"
rm -rf /etc/OpenCL/vendors/intel-beignet-x86_64-linux-gnu.icd
rm -rf /usr/include/CL/
rm -rf /usr/lib/x86_64-linux-gnu/beignet/

echo "Remove packages"
apt-get remove -y librealsense-all-dev=0.0.1~rc0-0ubuntu2~16.gbp3bd28d
apt-get auto-remove -y

echo "Remove apt file"
rm -rf /etc/apt/sources.list.d/realsense-public.list

apt-get update
