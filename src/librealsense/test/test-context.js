// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const assert = require('assert');
const addon = require('bindings')('node_librealsense');

describe('Test Context interfaces', function() {
  let context = new addon.Context;

  it('Get the device count', function() {
    return new Promise((resolve, reject) => {
      context.getDeviceCount().then((count) => {
        assert.equal(typeof count, 'number');
        console.log('device count equals to ' + count);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get device', function() {
    return new Promise((resolve, reject) =>{
      context.getDevice(0).then((device) => {
        assert(device instanceof addon.Device);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });
});
