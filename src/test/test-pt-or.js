// Copyright (c) 2017 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

// eslint-disable-next-line
'use strict';

/* global describe, it */
const orModule = require('../object-recognition');
const ptModule = require('../person-tracking');

describe('Multiple node-realsense addons test suite', function() {
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
      return or.start();
    });
  }

  function runPT(pt) {
    // console.log('run PT');
    return pt.start();
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

  it('Make sure OR + PT can be run/stop at (almost) the same time', function() {
    // eslint-disable-next-line
    this.timeout(15 * 1000);
    return Promise.all([verifyOR(100), verifyPT(100)]);
  });

  it('Make sure OR can be still working after PT is stopped', function() {
    // eslint-disable-next-line
    this.timeout(15 * 1000);
    return Promise.all([verifyOR(100), verifyPT(50)]);
  });

  it('Make sure PT can be still working after OR is stopped', function() {
    // eslint-disable-next-line
    this.timeout(15 * 1000);
    return Promise.all([verifyOR(50), verifyPT(100)]);
  });

  it('Make sure PT can be started while OR is running', function() {
    // eslint-disable-next-line
    this.timeout(30 * 1000);
    return Promise.all([delayRunOR(240, 0), delayRunPT(120, 5000)]);
  });

  it('Make sure OR can be started while PT is running', function() {
    // eslint-disable-next-line
    this.timeout(30 * 1000);
    return Promise.all([delayRunOR(120, 5000), delayRunPT(240, 0)]);
  });
});
