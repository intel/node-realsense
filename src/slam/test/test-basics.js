// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const spawnNpmInstall = require('spawn-npm-install');
const testNpmInstall = require('../common/scripts/test-npm-install.js');
const tmp = require('tmp');

const assert = require('assert');
const EventEmitter = require('events').EventEmitter;
const addon = require('bindings')('realsense_slam');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.Instance, EventEmitter);

describe('SLAM Test Suite - Basics', function() {
  it('Make the tarball', function() {
    return testNpmInstall.makeTarball();
  });

  it('Make sure can be installed via npm install <tarball>', function() {
    // eslint-disable-next-line
    this.timeout(200*1000);

    return testNpmInstall.installFromTarball(tmp, spawnNpmInstall);
  });
});

describe('Slam Test Suite -Run', function() {
  it('Test creating slam object using Promise', function() {
    return new Promise((resolve, reject) => {
      let promise = addon.createInstance();
      assert.equal(typeof promise, 'object');
      assert(promise instanceof Promise);

      promise.then((instance) => {
        assert.equal(typeof instance, 'object');
        assert(instance instanceof addon.Instance);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });
  it('Make sure properties are all there', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance().then((instance) => {
        assert(instance.hasOwnProperty('state'));
        assert.equal(Object.getOwnPropertyDescriptor(instance, 'state').writable, false);
        resolve();
      });
    });
  });

  it('Make sure methods are all there', function(done) {
    let Instance = addon.Instance;
    [
      'getCameraOptions', 'getInstanceOptions', 'setCameraOptions', 'setInstanceOptions',
      'start', 'stop', 'getTrackingResult', 'getOccupancyMap', 'getRelocalizationPose',
      'getOccupancyMapUpdate', 'getOccupancyMapBounds', 'loadOccupancyMap', 'saveOccupancyMap',
      'saveOccupancyMapAsPpm', 'loadRelocalizationMap', 'saveRelocalizationMap',
      // not support yet
      // 'resetConfig', 'restart',
    ].forEach((methodName) => {
      assert.equal(typeof Instance.prototype[methodName], 'function');
    });
    done();
  });
  it('createInstance() should resolve when providing 1 valid but empty argument', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance({}).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createInstance() should resolve when providing 2 valid but empty arguments (1)', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance({}, {}).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createInstance() should resolve when providing 2 valid but empty arguments (2)', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(undefined, {}).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createInstance() should resolve when providing 2 valid but empty arguments (3)', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(undefined, undefined).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createInstance() should resolve when providing 2 valid but empty arguments (4)', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance({}, undefined).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createInstance() should resolve when providing 2 valid but empty arguments (5)', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(null, null).then((i) => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it.skip('createInstance() should reject when providing 1 undefined argument', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(undefined).then((i) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it.skip('createInstance() should reject when providing 1 string argument', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance('dummy').then((i) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it.skip('createInstance() should reject when providing 1 number argument', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(89).then((i) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it.skip('createInstance() should reject when providing 2 string arguments', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance('humpty', 'dumpty').then((i) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it.skip('createInstance() should reject when providing 2 number arguments', function() {
    return new Promise((resolve, reject) => {
      addon.createInstance(89, 64).then((i) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });
});
