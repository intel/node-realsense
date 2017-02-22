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

describe('LocalizationInfo Test Suite - Run', function() {
  before(function() {
    this.timeout(50000);
    let or = undefined;
    return new Promise((resolve, reject) => {
      const options = {
          mode: 'localization',
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
        or.on('localization', function(data) {
          obj = data[0];
          if (or) {
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
  it('LocalizationInfo object has an attribute named label, type is String', function() {
    assert.equal(typeof(obj.label), 'string');
  });

  it('label of LocalizationInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'label').writable, false);
  });

  it('LocalizationInfo object has an attribute named probability, type is float', function() {
    assert.equal(typeof(obj.probability), 'number');
  });

  it('probability of LocalizationInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'probability').writable, false);
  });

  it('LocalizationInfo object has an attribute named roi, type is object', function() {
    assert.equal(typeof(obj.roi), 'object');
  });

  it('roi of LocalizationInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'roi').writable, false);
  });

  it('LocalizationInfo object has an attribute named objectCenter, type is object', function() {
    assert.equal(typeof(obj.objectCenter), 'object');
  });

  it('objectCenter of LocalizationInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'objectCenter').writable, false);
  });
});
