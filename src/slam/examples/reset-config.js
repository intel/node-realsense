// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
'use strict';

let slamAddon = require('bindings')('realsense_slam');
let EventEmitter = require('events').EventEmitter;

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

/** The same work as JavaScript wrapper **/

function instanceHandler(slamInstance) {
  slamInstance.getInstanceOptions().then(function(defaultOptions) {
    console.log('Default instance options:', defaultOptions);

    let options = {
      enableOccupancyMapBuilding: true,
      occupancyMapHeightOfInterest: {max: 1, min: -0.2},
      occupancyMapResolution: 0.2,
      enableRelocalizationMapping: false,
      enableForceRelocalizationPose: false,
    };
    return slamInstance.setInstanceOptions(options);
  })
  .then(function() {
    console.log('set instance options succeed');
    return slamInstance.getInstanceOptions();
  })
  .then(function(o) {
    console.log('Instance options after setInstanceOption:', o);
    return slamInstance.reset();
  })
  .then(function() {
    console.log('reset succeed');
    return slamInstance.getInstanceOptions();
  })
  .then(function(o) {
    console.log('Instance options after reset:', o);
    slamInstance.stop();
  })
  .then(function(o) {
    process.exit();
  })
  .catch(function(e) {
    console.log('error:' + e);
    process.exit();
  });
}

function test1() {
  slamAddon.createInstance().then(function(slamInstance) {
    console.log('In JS: Succeed to create slamInstance.');
    try {
      instanceHandler(slamInstance);
    } catch(e) {
      console.error('Error caught:', e);
      slamInstance.stop();
    }
  }, function(e) {
    console.error('In JS: Failed to create SLAM instance:' + e);
  });
}

test1();

process.on('uncaughtException', function(err) {
  console.error('Error caught in uncaughtException event:', err);
});
