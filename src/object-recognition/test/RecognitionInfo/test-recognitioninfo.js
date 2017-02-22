// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
/* eslint no-invalid-this: "off" */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');

const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.ObjectRecognizer, EventEmitter);

let obj;

describe('RecognitionInfo Test Suite - Run', function() {
  before(function() {
    let or = undefined;
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      let eventCounter = 0;
      const options = {
          mode: 'single-recognition',
          confidenceThreshold: 0.15,
          computeEngine: 'CPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: false,
      };
      addon.createObjectRecognizer(options).then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setObjectRecognitionOptions(options);
      })
      .then(() => {
        or.on('recognition', function(data) {
          obj = data[0];
          if (or && eventCounter++ > 2) {
            or.stop()
            .then(function() {
              resolve();
            });
            or = undefined;
          }
        });
        return or.start();
      })
      .catch((e) => {
        reject(e);
      });
    });
  });
  it('BoundingBox2DInfo object has an attribute named label, type is String', function() {
    assert.equal(typeof(obj.label), 'string');
  });
  it('label of BoundingBox2DInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'label').writable, false);
  });

  it('BoundingBox2DInfo object has an attribute named probability, type is float', function() {
    assert.equal(typeof(obj.probability), 'number');
  });
  it('probability of BoundingBox2DInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'probability').writable, false);
  });
});
