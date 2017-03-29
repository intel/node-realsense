// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('bindings')('node_librealsense');

describe('Test Context interfaces', function() {
  let context = new addon.Context;

  it('Negative - Call getDeviceCount with 1 option', function() {
    return new Promise((resolve, reject) => {
      context.getDeviceCount('dummy').then((count) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve(e);
      });
    });
  });
});

