// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
'use strict';

let slamAddon = require('bindings')('realsense_slam');
let EventEmitter = require('events').EventEmitter;
let trackingEvent = 'tracking';
let errorEvent = 'error';

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

function addListeners(instance) {
  instance.on('newListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = true;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = true;
  });
  instance.on('removeListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = false;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = false;
  });
}
/** The same work as JavaScript wrapper **/

function instanceHandler(slamInstance) {
  addListeners(slamInstance);
  let options = {
    enableOccupancyMapBuilding: true,
    occupancyMapHeightOfInterest: {max: 1, min: -0.2},
    occupancyMapResolution: 0.2,
    enableRelocalizationMapping: false,
    enableForceRelocalizationPose: false,
  };
  slamInstance.setInstanceOptions(options).then(function() {
    console.log('set instance options succeed');
    return slamInstance.getInstanceOptions();
  })
  .then(function(o) {
    console.log('options:', o);
   // slamInstance.addListener('tracking', function(e) {
   //   console.log('on tracking:', e);
   //   slamInstance.getTrackingResult().then((frameData) => {
   //     console.log('trackingResult:', frameData);
  // }, (e) => {
   //     console.log('failed to get trackingResult:', e);
    // });
   // });
    return slamInstance.start();
  })
  .then(function() {
    console.log('start succeed.');
  })
  .catch(function(e) {
    console.log('error:' + e);
  });

  var id = setInterval(getOccupancyMapUpdate, 5000);
  function getOccupancyMapUpdate() {
    if(id == 30) {
      clearInterval(id);
    } else {
      slamInstance.getOccupancyMapUpdate().then(function(mapData) {
        console.log('JS:get occupancy map success, data.length:' + mapData.data.length);
        console.log(mapData);
      })
      .catch(function(e) {
        console.log('error:' + e);
      });
    }
  }
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
