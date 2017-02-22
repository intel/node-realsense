 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

function argumentTest(obj, name, type, writable) {
  let typeResult = true;
  let writableResult = true;
  if(typeof(obj[name]) !== type) {
    typeResult = false;
  }
  if(Object.getOwnPropertyDescriptor(obj, name).writable !== writable) {
    writableResult = false;
  }
  return typeResult && writableResult;
}

function test(args0, args1) {
  describe('Slam Instance Test - getOccupancyMapAsRgba', function() {
    let slamInstance = null;
    after(function() {
      return slamInstance.stop();
    });

    it('Should get correct mapImage data by options: ' + args0 + ' and ' + args1, function() {
      // eslint-disable-next-line no-invalid-this
      this.timeout(60 * 1000);
      return new Promise((resolve, reject) => {
        slamAddon.createInstance().then((Instance) => {
          slamInstance = Instance;
          return slamInstance.start();
        }).then(() => {
          return slamInstance.getOccupancyMapAsRgba(args0, args1);
        }).then((mapImage) => {
          assert.ok(argumentTest(mapImage, 'width', 'number', false));
          assert.ok(argumentTest(mapImage, 'height', 'number', false));
          assert.ok(argumentTest(mapImage, 'data', 'object', false));
          resolve();
        }).catch((err) => {
          reject(err);
        });
      });
    });
  });
}

let testSets = {
  test1: {drawPoseTrajectory: true, drawOccupancyMap: true},
  test2: {drawPoseTrajectory: true, drawOccupancyMap: false},
  test3: {drawPoseTrajectory: false, drawOccupancyMap: true},
  test4: {drawPoseTrajectory: false, drawOccupancyMap: false},
};
// eslint-disable-next-line guard-for-in
for(let i in testSets) {
  test(testSets[i].drawPoseTrajectory, testSets[i].drawOccupancyMap);
}
