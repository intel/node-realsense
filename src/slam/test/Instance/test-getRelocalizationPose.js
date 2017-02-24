 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

describe('Slam Instance Test - getRelocalizationPose()', function() {
  let slamInstance = null;
  afterEach(() => {
    return slamInstance.stop();
  });
  it.skip('Should get correct data getRelocalizationPose() - Positive -  ', function() {
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((Instance) => {
        slamInstance = Instance;
        return slamInstance.start();
      }).then(() => {
          return slamInstance.getRelocalizationPose();
      }).then((Data) => {
        assert.equal(typeof(Data), 'object');
        assert.equal(typeof(Data[0], 'number'));
        resolve();
      }).catch((err) => {
        reject(err);
      });
    });
  });
  it.skip('Should get correct data getRelocalizationPose() - Negative - ', function() {
    let argv = null;
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((Instance) => {
        slamInstance = Instance;
        return slamInstance.start();
      }).then(() => {
          return slamInstance.getRelocalizationPose(argv);
      }).then((Data) => {
        assert.equal(typeof(Data), 'object');
        assert.equal(typeof(Data[0], 'number'));
        resolve();
      }).catch((err) => {
        reject(err);
      });
    });
  });
});
