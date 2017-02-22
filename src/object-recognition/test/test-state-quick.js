// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');
const path = require('path');

const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.ObjectRecognizer, EventEmitter);

describe('Object Recognition Test Suite - State attribute', function() {
  let or = undefined;

  it('Create an or and check its state', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        assert.equal(or.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Run an or and check its state', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      const options = {
        mode: 'single-recognition',
        confidenceThreshold: 0.15,
        computeEngine: 'CPU',
        enableSegmentation: true,
        maxReturnObjectCount: 3,
        enableObjectCenterEstimation: false,
      };
      const cameraOptions = {
        color: {
          width: 640,
          height: 480,
          frameRate: 30,
          isEnabled: true,
        },
        depth: {
          width: 320,
          height: 240,
          frameRate: 30,
          isEnabled: true,
        },
        playbackPathForTesting: path.resolve(path.join(
            '..', 'common', 'tests', 'data', 'or',
            'or_zr300_sync_color-640x480-rgb8-30_depth-320x240-z16-30_frames-200.out')),
      };
      or.setCameraOptions(cameraOptions).then(() => {
        return or.setObjectRecognitionOptions(options);
      }).then(() => {
        return or.start();
      }).then(()=> {
        assert.equal(or.state, 'running');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Pause an or and check its state', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      or.pause().then(() => {
        assert.equal(or.state, 'paused');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Resume an or and check its state', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      or.resume().then(() => {
        assert.equal(or.state, 'running');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Stop an or and check its state', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      or.stop().then(() => {
        assert.equal(or.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });
});
