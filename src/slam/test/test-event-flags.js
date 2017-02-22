// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
//
'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_slam');
let EventEmitter = require('events').EventEmitter;
let trackingEvent = 'tracking';
let errorEvent = 'error';

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.Instance, EventEmitter);

function addListeners(instance) {
  instance.on('newListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = true;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = true;
  });
  instance.on('removeListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = false;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = false;
  });
}


describe('SLAM Test Suite - event flag', function() {
  let slamInstance = undefined;
  before(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      addon.createInstance().then(function(instance) {
        assert.equal(typeof (instance), 'object');
        slamInstance = instance;
        addListeners(slamInstance);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Test event flag', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      assert.equal(slamInstance.enableTrackingEvent, false);
      slamInstance.addListener('tracking', function() {
        console.log('on tracking');
      });
      assert.equal(slamInstance.enableTrackingEvent, true);
      slamInstance.removeAllListeners('tracking');
      assert.equal(slamInstance.enableTrackingEvent, false);
      slamInstance.stop();
      resolve();
    });
  });
});
