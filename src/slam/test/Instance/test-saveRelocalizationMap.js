 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const fs = require('fs');

function checkFileExist(file) {
  return new Promise((resolve, reject) => {
    fs.exists(file, function(exists) {
      if(exists) {
        fs.unlink(file, function(err) {
          if (err) {
            reject(err);
          }
        });
        resolve();
      }else{
        reject();
      }
    });
  });
}

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

function tests(state, fileName) {
  describe('Slam Instance Test - saveRelocalizationMap()', function() {
    let slamInstance = null;
    afterEach(() => {
      return slamInstance.stop();
    });
    it('Should get correct data saveRelocalizationMap() -  ' + state, function() {
      // eslint-disable-next-line
      this.timeout(5000);
      return new Promise((resolve, reject) => {
        slamAddon.createInstance().then((Instance) => {
          slamInstance = Instance;
          return slamInstance.start();
        }).then(() => {
          return slamInstance.saveRelocalizationMap(fileName);
        }).then(() => {
          return checkFileExist(fileName);
        }).then(() => {
          resolve();
        }).catch((err) => {
          reject(err);
        });
      });
    });
  });
}
let argv = '/tmp/saveRelocalizationMap.map';
tests('Positive - 1', argv);
