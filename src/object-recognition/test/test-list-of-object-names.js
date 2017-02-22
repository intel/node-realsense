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

describe('Object Recognition Test Suite - Names of Recognizable Objects', function() {
  it('Names can be retrieved after .start()', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);

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

    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        objectRecognizer.setCameraOptions(cameraOptions).then(() => {
          return objectRecognizer.start();
        }).then(() => {
          return objectRecognizer.getRecognizableObjectNames();
        }).then((names) => {
          assert(Array.isArray(names));
          assert(names.length > 0);
          return objectRecognizer.stop();
        }).then(() => {
          resolve();
        });
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Names cannot be retrieved before .start()', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        objectRecognizer.getRecognizableObjectNames().then((names) => {
          reject('Should not resolve since objectRecognizer is not started');
        }).catch((e) => {
          resolve();
        });
      }).catch((e) => {
        reject(e);
      });
    });
  });
});
