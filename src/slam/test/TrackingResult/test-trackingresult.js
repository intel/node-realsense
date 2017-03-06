 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');
const utils = require('../../common/tests/testUtils.js');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

describe('Slam Instance Test - TrackingResult', function() {
  let slamInstance = null;
  let trackingResult = null;
  after(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(30 * 1000);
    return slamInstance.stop();
  });

  before(function(done) {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    let fDone = false;
    slamAddon.createInstance().then((Instance) => {
      slamInstance = Instance;
      slamInstance.on('tracking', (result) => {
        slamInstance.getTrackingResult().then((result) => {
          if(result.frameData.fisheye
            // color is undefined for SLAM
            // && result.frameData.color
            && result.frameData.depth) {
            trackingResult = result;
            if (!fDone) {
              fDone = true;
              done();
            }
          }
        });
      });
      slamInstance.start();
    }).catch((err) => {
      console.log(err);
      done();
    });
  });
  it('Should get correct frameData data', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise((resolve, reject) => {
      if(utils.isFrameData(trackingResult.frameData) === true) {
        resolve();
      }
      reject(utils.isFrameData(trackingResult.frameData));
    });
  });
  it('Should get correct cameraPose data', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise((resolve, reject) => {
      let obj = trackingResult;
      assert(obj.hasOwnProperty('cameraPose'));
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'cameraPose').writable, false);
      assert.equal(typeof(obj.cameraPose), 'object');
      resolve();
    });
  });
  it('Should get correct accuracy data', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise((resolve, reject) => {
      let obj = trackingResult;
      assert(obj.hasOwnProperty('accuracy'));
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'accuracy').writable, false);
      assert.equal(typeof(obj.accuracy), 'string');
      resolve();
    });
  });
});
