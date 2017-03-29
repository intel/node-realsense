// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const addon = require('bindings')('realsense_object_recognition');
const assert = require('assert');
const spawnNpmInstall = require('spawn-npm-install');
const testNpmInstall = require('../common/scripts/test-npm-install.js');
const tmp = require('tmp');

describe('Object Recognition Test Suite - Basics', function() {
  it('Make the tarball', function() {
    return testNpmInstall.makeTarball();
  });

  it.skip('Make sure can be installed via npm install <tarball>', function() {
    // eslint-disable-next-line
    this.timeout(200*1000);

    return testNpmInstall.installFromTarball(tmp, spawnNpmInstall);
  });

  it('Test creating object-recognition object using Promise', function() {
    return new Promise((resolve, reject) => {
      let promise = addon.createObjectRecognizer();
      assert.equal(typeof promise, 'object');
      assert(promise instanceof Promise);

      promise.then((objectRecognizer) => {
        assert.equal(typeof objectRecognizer, 'object');
        assert(objectRecognizer instanceof addon.ObjectRecognizer);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Make sure properties are all there', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        assert(objectRecognizer.hasOwnProperty('state'));
        assert.equal(typeof objectRecognizer.state, 'string');
        assert.equal(Object.getOwnPropertyDescriptor(objectRecognizer, 'state').writable, false);
        resolve();
      });
    });
  });

  it('Make sure methods are all there', function(done) {
    let ObjectRecognizer = addon.ObjectRecognizer;
    [
      'getCameraOptions', 'setCameraOptions',
      'getObjectRecognitionOptions', 'setObjectRecognitionOptions',
      'start', 'pause', 'resume', 'stop', 'reset', 'restart',
    ].forEach((methodName) => {
      assert.equal(typeof ObjectRecognizer.prototype[methodName], 'function');
    });

    done();
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 1 valid but empty argument', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer({}).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 2 valid but empty arguments (1)', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer({}, {}).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 2 valid but empty arguments (2)', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(undefined, {}).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 2 valid but empty arguments (3)', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(undefined, undefined).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 2 valid but empty arguments (4)', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer({}, undefined).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('createObjectRecognizer() should resolve when providing 2 valid but empty arguments (5)', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(null, null).then(() => {
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected');
      });
    });
  });

  it('createObjectRecognizer() should reject when providing 1 undefined argument', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(undefined).then(() => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it('createObjectRecognizer() should reject when providing 1 string argument', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer('dummy').then(() => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it('createObjectRecognizer() should reject when providing 1 number argument', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(89).then(() => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it('createObjectRecognizer() should reject when providing 2 string arguments', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer('humpty', 'dumpty').then(() => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it('createObjectRecognizer() should reject when providing 2 number arguments', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(89, 64).then(() => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });
});
