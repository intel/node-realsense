#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#

#set -x
# Run script as root, unless permissions are dropped elsewhere."

echo "Please input the test lasting time (seconds): "
read num
END=`expr $num / 10`
cpu_log=cpu_used.dat
mem_log=mem_info.dat
gpu_log=gpu.dat
echo "" > gpu.dat
echo "#time cpu_used" > cpu_used.dat
echo "#time total free used buff/cache" > mem_info.dat
intel_gpu_top -o $gpu_log 2>&1 > /dev/null &
sleep 10
for ((i=1;i<=END;i++))
do
  top_info=`top -bn1 |grep -E "KiB Mem|$1"| paste -s -d "!"`
  cpu_used=`echo $top_info|awk -F '!' '{print $2}'|awk '{print $9}'|awk -F '.' '{print $1}'`
  mem_info=`echo $top_info|awk -F ' ' '{print $4,$6,$8,$10,$8/$4*100}'`
  n=`expr $i \* 10`
  echo $n $cpu_used >> $cpu_log
  echo $n $mem_info >> $mem_log
  remain=`expr $num - $i \* 10 + 10`
  h=`expr $remain / 3600`
  m=`expr $remain % 3600 / 60`
  s=`expr $remain % 60`
  echo "test remaining $h hour(s) $m minute(s) $s second(s)"
  sleep 10
done
gpu_pids=`ps aux | grep intel_gpu_top|grep -v grep| awk '{print $2}'`
for pid in $gpu_pids;do
  kill -9 $pid 2>&1 > /dev/null
done
echo "Done. result kept at $cpu_log, $mem_log and $gpu_log"

kill -9 $$
