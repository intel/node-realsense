 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');
const fs = require('fs');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

describe('Slam Instance Test - loadOccupancyMap()', function() {
  let argv = '/tmp/loadOccupancyMap.map';
  let slamInstance = null;
  afterEach(() => {
    fs.exists(argv, function(exists) {
      if (exists) {
        fs.unlink(argv, function(err) {
          if (err) {
            throw err;
          }
        });
      }
    });
    return slamInstance.stop();
  });
  it('Should get correct data loadOccupancyMap() -  Positive -  ', function() {
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((Instance) => {
        slamInstance = Instance;
        return slamInstance.start();
      }).then(() => {
        return slamInstance.saveOccupancyMap(argv);
      }).then(() => {
        return slamInstance.loadOccupancyMap(argv);
      }).then((Data) => {
        assert.equal(typeof(Data), 'undefined');
        resolve();
      }).catch((err) => {
        reject(err);
      });
    });
  });
  it('Should get correct data loadOccupancyMap() -  Negative - ', function() {
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((Instance) => {
        slamInstance = Instance;
        return slamInstance.start();
      }).then(() => {
        return slamInstance.saveOccupancyMap(argv);
      }).then(() => {
        return slamInstance.loadOccupancyMap();
      }).then((Data) => {
        reject('Promise should reject, but got resolved');
      }).catch((err) => {
        resolve();
      });
    });
  });
});
