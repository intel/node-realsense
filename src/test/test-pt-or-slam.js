// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// eslint-disable-next-line
'use strict';

/* global afterEach, describe, it */
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

describe('Multiple node-realsense addons test suite', function() {
  let gPt;
  let gSlam;
  afterEach(function() {
    return Promise.all([gPt.reset(), gSlam.reset()]);
  });
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

  function runPT(pt) {
    // console.log('run PT');
    return pt.setCameraOptions(cameraOptions).then(function() {
      return pt.start();
    });
  }

  function runSLAM(slam) {
    // console.log('run SLAM');
    return slam.setCameraOptions(cameraOptions).then(function() {
      return slam.start();
    });
  }

  function verifyOR(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      orModule.createObjectRecognizer().then(function(or) {
        or.on('recognition', function(eventData) {
console.log('or:' + counter);
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

  function verifyPT(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      ptModule.createPersonTracker().then(function(pt) {
        gPt = pt;
        pt.on('frameprocessed', function(result) {
console.log('pt:' + counter);
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

  function verifySLAM(runNumOfFrames) {
    return new Promise(function(resolve, reject) {
      let counter = 0;
      slamModule.createInstance().then(function(slam) {
        gSlam = slam;
        slam.on('tracking', function(eventData) {
console.log('slam:' + counter);
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

  it('Make sure OR + PT + SLAM can be run/stop at (almost) the same time', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([verifyOR(20), verifyPT(100), verifySLAM(100)]);
  });

  // Block by #221
  it.skip('Make sure OR can be still working after PT and SLAM are stopped', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([verifyOR(20), verifyPT(50), verifySLAM(50)]);
  });

  // Block by #221
  it.skip('Make sure PT can be still working after OR and SLAM are stopped', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([verifyOR(20), verifyPT(100), verifySLAM(50)]);
  });

  // Block by #221
  it.skip('Make sure SLAM can be still working after OR and PT are stopped', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([verifyOR(20), verifyPT(50), verifySLAM(500)]);
  });

  // Block by #221
  it.skip('Make sure PT can be started while OR and SLAM are running', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([delayRunOR(20, 0), delayRunPT(120, 5000), delayRunSLAM(240, 0)]);
  });

  // Block by #221
  it.skip('Make sure OR can be started while PT and SLAM are running', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([delayRunOR(20, 5000), delayRunPT(240, 0), delayRunSLAM(240, 0)]);
  });

  // Block by #221
  it.skip('Make sure SLAM can be started while PT and OR are running', function() {
    // eslint-disable-next-line
    this.timeout(90 * 1000);
    return Promise.all([delayRunOR(20, 0), delayRunPT(240, 0), delayRunSLAM(120, 5000)]);
  });
});
