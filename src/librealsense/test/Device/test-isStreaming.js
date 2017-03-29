// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const assert = require('assert');
const addon = require('bindings')('node_librealsense');

describe('Test Device interfaces', function() {
  let context = new addon.Context;
  let device = null;
  let mode = {
    width: 640,
    height: 480,
    format: 'rgb',
    framerate: 30,
  };

  before(function(done) {
    context.getDevice(0).then((availableDevice) => {
      device = availableDevice;
      done();
    });
  });

  it('Negative - Call isStreaming with 1 option', function() {
    return new Promise((resolve, reject) => {
      device.isStreaming('dummy').then((name) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Positive - isStreaming got ture after call enableStream function', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(30000);
    return new Promise((resolve, reject) => {
      device.isStreaming().then((enable) => {
        assert.equal(enable, false);
        return device.enableStream('color', mode);
      });
      device.enableStream('color', mode).then(() => {
        return device.start();
      }).then(() => {
        return device.isStreaming();
      }).then((enable) => {
        assert.equal(enable, true);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });
});
