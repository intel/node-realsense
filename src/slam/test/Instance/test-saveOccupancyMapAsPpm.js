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

function tests(state, options1, options2) {
  describe('Slam Instance Test - saveOccupancyMapAsPpm()', function() {
    let slamInstance = null;
    afterEach(() => {
      return slamInstance.stop();
    });
    it('Should get correct data saveOccupancyMapAsPpm() -  ' + state, function() {
      return new Promise((resolve, reject) => {
        slamAddon.createInstance().then((Instance) => {
          slamInstance = Instance;
          return slamInstance.start();
        }).then(() => {
            if (options1) {
              return slamInstance.saveOccupancyMapAsPpm(options1, options2);
            }else{
              return slamInstance.saveOccupancyMapAsPpm();
            }
        }).then((Data) => {
          fs.exists(Data, function(exists) {
            if (exists) {
              fs.unlink(Data, function(err) {
                if (err) {
                  throw err;
                }
              });
              assert.ok(Data);
            }
          });
          resolve();
        }).catch((err) => {
          reject(err);
        });
      });
    });
  });
}
let argv = '/tmp/saveOccupancyMapAsPpm.map';
tests('Positive - 1 ', argv, false);
tests('Positive - 2', argv, true);
tests('Positive - 3', argv, null);
// tests('Negative')
// tests('Negative', null, true)
// tests('Negative', null, false)
// tests('Negative', null, null)
