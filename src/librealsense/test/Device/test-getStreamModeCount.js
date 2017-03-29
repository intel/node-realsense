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

  it('Negative - Call getStreamModeCount with no option', function() {
    return new Promise((resolve, reject) => {
      device.getStreamModeCount().then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call getStreamModeCount with 2 options', function() {
    return new Promise((resolve, reject) => {
      device.getStreamModeCount('depth', 'color').then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call getStreamModeCount with illegal options', function() {
    return new Promise((resolve, reject) => {
      device.getStreamModeCount('dummy').then((enable) => {
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

    before(function(done) {
      context.getDevice(0).then((availableDevice) => {
        device = availableDevice;
        done();
      });
    });

    it('Positive - Call getStreamModeCount with ' + option, function() {
      return new Promise((resolve, reject) => {
        device.getStreamModeCount(option).then((number) => {
          assert.equal(typeof number, 'number');
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
  'points',
  'rectified_color',
  'color_aligned_to_depth',
  'infrared2_aligned_to_depth',
  'depth_aligned_to_color',
  'depth_aligned_to_rectified_color',
  'depth_aligned_to_infrared2',
];
// eslint-disable-next-line guard-for-in
for(let i in testSet) {
  testOptions(testSet[i]);
}
