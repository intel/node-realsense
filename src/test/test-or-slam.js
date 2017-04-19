// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// eslint-disable-next-line
'use strict';

/* global describe, it */
const orModule = require('../object-recognition');
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

describe('Multiple node-realsense addons test suite', function() {
  let gSlam;
  afterEach(function() {
    return gSlam.reset();
  });

  function runSLAM(slam) {
    // console.log('run SLAM');
    return slam.setCameraOptions(cameraOptions).then(function() {
      return slam.start();
    });
  }

  function runOR(or) {
    return or.setObjectRecognitionOptions({
      mode: 'single-recognition',
      confidenceThreshold: 0.3,
      computeEngine: 'CPU',
      enableSegmentation: true,
      maxReturnObjectCount: 2,
      enableObjectCenterEstimation: false,
    }).then(function() {
      // console.log('run OR');
      return or.setCameraOptions(cameraOptions).then(function() {
        return or.start();
      });
    });
  }

  function verifySLAM(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      slamModule.createInstance().then(function(slam) {
        gSlam = slam;
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

  function verifyOR(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      orModule.createObjectRecognizer().then(function(or) {
        or.on('recognition', function(eventData) {
          if (counter >= 0) {
            ++ counter;
          }

          if (counter > runNumOfFrames) {
            counter = -1;
            // console.log('stopping OR');
            or.stop().then(function() {
              // console.log('OR is stopped');
              resolve('Yes, OR works');
            }).catch(function(e) {
              reject(e);
            });
          }
        });

        return runOR(or).catch(function(e) {
          reject('Failed to run OR');
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

  function delayRunOR(runNumOfFrames, delay) {
    return new Promise(function(resolve, reject) {
      setTimeout(function() {
        verifyOR(runNumOfFrames).then(function(v) {
          resolve(v);
        }).catch(function(e) {
          reject(e);
        });
      }, delay);
    });
  }

  it('Make sure SLAM + OR can be run/stop at (almost) the same time', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([verifySLAM(100), verifyOR(20)]);
  });

  it('Make sure SLAM can be still working after OR is stopped', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([verifySLAM(200), verifyOR(20)]);
  });

  it('Make sure OR can be still working after SLAM is stopped', function() {
    // eslint-disable-next-line
    this.timeout(30 * 1000);
    return Promise.all([verifySLAM(50), verifyOR(20)]);
  });

  it('Make sure OR can be started while SLAM is running', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([delayRunSLAM(240, 0), delayRunOR(20, 5000)]);
  });

  it('Make sure SLAM can be started while OR is running', function() {
    // eslint-disable-next-line
    this.timeout(60 * 1000);
    return Promise.all([delayRunSLAM(120, 5000), delayRunOR(20, 0)]);
  });
});
