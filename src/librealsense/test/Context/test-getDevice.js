// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('bindings')('node_librealsense');

describe('Test Context interfaces', function() {
  let context = new addon.Context;

  it('Negative - Call getDeviceCount with no option', function() {
    return new Promise((resolve, reject) => {
      context.getDevice().then((count) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve(e);
      });
    });
  });

  it('Negative - Call getDeviceCount with 2 options', function() {
    return new Promise((resolve, reject) => {
      context.getDevice(0, 0).then((count) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve(e);
      });
    });
  });


  it('Negative - Call getDeviceCount with strint option', function() {
    return new Promise((resolve, reject) => {
      context.getDevice('1').then((count) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve(e);
      });
    });
  });

  it('Negative - Call getDeviceCount with option index 1 (not exist)', function() {
    return new Promise((resolve, reject) => {
      context.getDevice(1).then((count) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve(e);
      });
    });
  });
});

