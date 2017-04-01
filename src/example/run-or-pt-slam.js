// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// eslint-disable-next-line
'use strict';

const orModule = require('../object-recognition');
const ptModule = require('../person-tracking');
const slamModule = require('../slam');

const cameraOptions = {
  color: {
    width: 640,
    height: 480,
    frameRate: 30,
    isEnabled: true,
  },
  fisheye: {
    width: 640,
    height: 480,
    frameRate: 30,
    isEnabled: true,
  },
  depth: {
    width: 320,
    height: 240,
    frameRate: 30,
    isEnabled: true,
  },
};

function runOR(timeout) {
  return orModule.createObjectRecognizer().then((or) => {
    or.on('frameprocessed', (eventData) => {
      console.log('OR: frameprocessed');
    });

    or.on('recognition', (eventData) => {
      console.log('OR: recognition', eventData);
    });

    return or.setObjectRecognitionOptions({
      mode: 'single-recognition',
      confidenceThreshold: 0.35,
      computeEngine: 'CPU',
      enableSegmentation: true,
      maxReturnObjectCount: 2,
      enableObjectCenterEstimation: false,
    }).then(() => {
      return or.setCameraOptions(cameraOptions);
    }).then(() => {
      return or.start().then(() => {
        if (timeout) {
          setTimeout(() => {
            console.log('-------------------------------------------------');
            console.log('Timeout, stopping object-recognition module.');
            console.log('-------------------------------------------------');
            or.stop();
          }, timeout);
        }
      });
    });
  });
}

function runPT(timeout) {
  return ptModule.createPersonTracker().then((pt) => {
    pt.on('persontracked', (result) => {
      console.log('PT: persontracked, number of persons:', result.persons.length);
    });

    pt.on('frameprocessed', (result) => {
      console.log('PT: frameprocessed');
    });

    return pt.setCameraOptions(cameraOptions).then(() => {
      return pt.start().then(() => {
        if (timeout) {
          setTimeout(() => {
            console.log('-------------------------------------------------');
            console.log('Timeout, stopping person-tracking module.');
            console.log('-------------------------------------------------');
            pt.stop();
          }, timeout);
        }
      });
    });
  });
}

function processMap(slam) {
  slam.getOccupancyMapUpdate().then((mapData) => {
    console.log('SLAM: getOccupancyMapUpdate, data.length:' + mapData.data.length);
    console.log(mapData);
  }).catch((e) => {
    console.log('SLAM: getOccupancyMapUpdate error:' + e);
  });
}

function runSLAM() {
  slamModule.createInstance().then((slam) => {
    slam.on('tracking', (eventData) => {
      console.log('SLAM: ontracking: ', eventData);
    });

    slam.on('error', (eventData) => {
      console.log('SLAM: onerror: ', eventData);
    });

    slam.setInstanceOptions({
      enableOccupancyMapBuilding: true,
      occupancyMapHeightOfInterest: {max: 1, min: -0.2},
      occupancyMapResolution: 0.2,
      enableRelocalizationMapping: false,
      enableForceRelocalizationPose: false,
    }).then(() => {
      return slam.setCameraOptions(cameraOptions);
    }).then(() => {
      console.log('SLAM: starting');
      slam.start().then(() => {
        processMap(slam);
        console.log('SLAM: started');
      });
    });
  }).catch((e) => {
    console.log('SLAM exception:', e);
  });
}

runOR();
runPT();
runSLAM();
