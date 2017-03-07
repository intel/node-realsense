// Copyright (c) 2016 Intel Corporation. All rights reserved.
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
    slamInstance.addListener('tracking', function(e) {
      console.log('on tracking:', e);
      slamInstance.getTrackingResult().then((frameData) => {
        console.log('trackingResult:', frameData);
      }, (e) => {
        console.log('failed to get trackingResult:', e);
      });
    });
    slamInstance.addListener('error', function(e) {
       console.log('on error:', e);
    });
    return slamInstance.start();
  })
  .then(function() {
    setInterval(() => {
      getOccupancyMapUpdate(slamInstance);
      getOccupancyMapBounds(slamInstance);
      getOccupancyMapAsRgba(slamInstance);
    }, 5000);
    console.log('start succeed.');
  })
  .catch(function(e) {
    console.log('error:' + e);
  });

  function saveOccupancyMapAsPpm() {
    slamInstance.saveOccupancyMapAsPpm( __dirname + '/map.ppm', false)
    .then(function() {
      console.log('JS:save occupancy map as ppm success.');
    })
    .catch(function(e) {
      console.log('error:' + e);
    });
  }

  setTimeout(saveOccupancyMapAsPpm, 10000);
}

let id = 0;
function getOccupancyMapUpdate(slamInstance) {
  if(id == 10) {
    clearInterval(id);
  } else {
    slamInstance.getOccupancyMapUpdate().then(function(mapData) {
      console.log('JS:get occupancy map success, data.length:' + mapData.data.length);
      console.log(mapData);
    })
    .catch(function(e) {
      console.log('error:' + e);
    });
    id++;
  }
}

function getOccupancyMapAsRgba(slamInstance) {
  if(id == 10) {
    clearInterval(id);
  } else {
    slamInstance.getOccupancyMapAsRgba(true, true).then(function(mapImage) {
      console.log('JS:get occupancy map success,');
      console.log('width: ' + mapImage.width);
      console.log('height: ' + mapImage.height);
      console.log('data: ' + mapImage.data);
    })
    .catch(function(e) {
      console.log('error:' + e);
    });
    id++;
  }
}

function getOccupancyMapBounds(slamInstance) {
  if(id == 10) {
    clearInterval(id);
  } else {
    slamInstance.getOccupancyMapBounds().then(function(boundsData) {
      console.log('JS:get occupancy map bounds success');
      console.log(boundsData);
    })
    .catch(function(e) {
      console.log('error:' + e);
    });
    id++;
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
