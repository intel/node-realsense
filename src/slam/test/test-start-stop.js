// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_slam');
let EventEmitter = require('events').EventEmitter;

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.Instance, EventEmitter);

describe('SLAM Test Suite - start-stop', function() {
  let slamInstance = undefined;
  before(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      addon.createInstance().then(function(instance) {
        assert.equal(typeof (instance), 'object');
        slamInstance = instance;
        assert.equal(slamInstance.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Test start-stop-start-stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      slamInstance.start().then(function() {
        assert.equal(slamInstance.state, 'tracking');
        return slamInstance.stop();
      }, function(e) {
        console.log('start error:' + e);
        reject(e);
      })
      .then(function() {
        // Start SLAM again.
        assert.equal(slamInstance.state, 'ready');
        return slamInstance.start();
      }, function(e) {
        console.log('stop error:' + e);
        reject(e);
      })
      .then(function() {
        assert.equal(slamInstance.state, 'tracking');
        return slamInstance.stop();
      }, function(e) {
        console.log('start again error:' + e);
        reject(e);
      })
      .then(function() {
        assert.equal(slamInstance.state, 'ready');
        resolve();
      }, function(e) {
        console.log('stop again error:' + e);
        reject(e);
      });
    });
  });
});
