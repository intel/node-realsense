// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
'use strict';

let slamAddon = require('bindings')('realsense_slam');

let instance;
function test1() {
  slamAddon.createInstance()
  .then(function(slamInstance) {
    instance = slamInstance;
    console.log('In JS: Succeed to create slamInstance.');
    return slamInstance.start();
  }).then(function() {
    console.log('In JS: Succeed to start.');
    return instance.saveOccupancyMap(__dirname + '/map');
  }).then(function() {
    console.log('success save occupancy map');
    return instance.loadOccupancyMap(__dirname + '/map');
  }).then(function() {
    console.log('success load occupancy map');
    return slamInstance.stop();
  });
}

test1();

process.on('uncaughtException', function(err) {
  console.error('Error caught in uncaughtException event:', err);
});
