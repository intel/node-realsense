// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// eslint-disable-next-line
'use strict';

/* global describe, it */
const ptModule = require('../person-tracking');
const slamModule = require('../slam');
const cameraOptions = {
  fisheye: {
    width: 640,
    height: 480,
    frameRate: 30,
    isEnabled: true,
  },
};

describe('Multiple node-realsense addons test suite', function() {
  function runSLAM(slam) {
    // console.log('run SLAM');
    return slam.setCameraOptions(cameraOptions).then(function() {
      return slam.start();
    });
  }

  function runPT(pt) {
    // console.log('run PT');
    return pt.setCameraOptions(cameraOptions).then(function() {
      return pt.start();
    });
  }

  function verifySLAM(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      slamModule.createInstance().then(function(slam) {
        slam.on('tracking', function(eventData) {
          if (counter >= 0) {
            ++ counter;
          }

          if (counter > runNumOfFrames) {
            counter = -1;
            // console.log('stopping SLAM');
            slam.stop().then(function() {
              // console.log('SLAM is stopped');
              resolve('Yes, SLAM works');
            }).catch(function(e) {
              reject(e);
            });
          }
        });

        return runSLAM(slam).catch(function(e) {
          reject('Failed to run SLAM');
        });
      }).catch(function(e) {
        reject(e);
      });
    });
  }

  function verifyPT(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      ptModule.createPersonTracker().then(function(pt) {
        pt.on('frameprocessed', function(result) {
          if (counter >= 0) {
            ++ counter;
          }

          if (counter > runNumOfFrames) {
            counter = -1;
            // console.log('stopping PT');
            pt.stop().then(function() {
              // console.log('PT is stopped');
              resolve('Yes, PT works');
            }).catch(function(e) {
              reject(e);
            });
          }
        });

        return runPT(pt).catch(function(e) {
          reject('Failed to run PT');
        });
      });
    });
  }

  function delayRunSLAM(runNumOfFrames, delay) {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        verifySLAM(runNumOfFrames).then(function(v) {
          resolve(v);
        }).catch(function(e) {
          reject(e);
        });
      }, delay);
    });
  }

  function delayRunPT(runNumOfFrames, delay) {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        verifyPT(runNumOfFrames).then(function(v) {
          resolve(v);
        }).catch(function(e) {
          reject(e);
        });
      }, delay);
    });
  }

  it('Make sure SLAM + PT can be run/stop at (almost) the same time', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([verifySLAM(100), verifyPT(100)]);
  });

  it('Make sure SLAM can be still working after PT is stopped', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([verifySLAM(100), verifyPT(50)]);
  });

  it('Make sure PT can be still working after SLAM is stopped', function() {
    // eslint-disable-next-line
    this.timeout(30 * 1000);
    return Promise.all([verifySLAM(50), verifyPT(100)]);
  });

  it('Make sure PT can be started while SLAM is running', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([delayRunSLAM(240, 0), delayRunPT(120, 5000)]);
  });

  it('Make sure SLAM can be started while PT is running', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([delayRunSLAM(120, 5000), delayRunPT(240, 0)]);
  });
});
