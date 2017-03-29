// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const assert = require('assert');
const addon = require('bindings')('node_librealsense');

describe('Test Device interfaces', function() {
  let context = new addon.Context;
  let device = null;

  before(function(done) {
    context.getDevice(0).then((availableDevice) => {
      device = availableDevice;
      done();
    });
  });

  it('Negative - Call getStreamHeight with no option', function() {
    return new Promise((resolve, reject) => {
      device.getStreamHeight().then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call getStreamHeight with 2 options', function() {
    return new Promise((resolve, reject) => {
      device.getStreamHeight('depth', 'color').then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call getStreamHeight with illegal options', function() {
    return new Promise((resolve, reject) => {
      device.getStreamHeight('dummy').then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });
});

function testOptions(option) {
  describe('Test Device interfaces', function() {
    let context = new addon.Context;
    let device = null;
    let mode = {
      width: 640,
      height: 480,
      format: 'any',
      framerate: 30,
    };

    before(function(done) {
      context.getDevice(0).then((availableDevice) => {
        device = availableDevice;
        done();
      });
    });

    it('Positive - Call getStreamHeight with ' + option, function() {
      return new Promise((resolve, reject) => {
        device.enableStream(option, mode).then(() => {
          return device.getStreamHeight(option);
        }).then((height) => {
          assert.equal(height, mode.height);
          resolve();
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
}

let testSet = [
  'depth',
  'color',
  'infrared',
  'infrared2',
  // 'points',
  // 'rectified_color',
  // 'color_aligned_to_depth',
  // 'infrared2_aligned_to_depth',
  // 'depth_aligned_to_color',
  // 'depth_aligned_to_rectified_color',
  // 'depth_aligned_to_infrared2'
];
// eslint-disable-next-line guard-for-in
for(let i in testSet) {
  testOptions(testSet[i]);
}
