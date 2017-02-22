// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

let slamAddon = require('bindings')('realsense_slam');
let EventEmitter = require('events').EventEmitter;
let trackingEvent = 'tracking';
let errorEvent = 'error';
let slamWrapper = {};

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

slamWrapper.createInstance = function() {
  return new Promise(function(resolve, reject) {
    slamAddon.createInstance().then(function(instance) {
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
      resolve(instance);
    }, function(e) {
      reject(e);
    });
  });
};

module.exports = slamWrapper;
