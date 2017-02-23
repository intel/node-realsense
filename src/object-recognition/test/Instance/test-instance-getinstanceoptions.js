// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
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

let or = undefined;

const orOptionsPositive = {
    mode: {
      value: ['single-recognition', 'tracking', 'localization'],
      type: 'string',
    },
    framesSkipping: {
      value: [0, 60, 2147483647],
      type: 'number',
    },
    confidenceThreshold: {
      value: [0, 0.15, 0.75, 1],
      type: 'number',
    },
    computeEngine: {
      value: ['CPU', 'GPU'],
      type: 'string',
    },
    enableSegmentation: {
      value: [true, false],
      type: 'boolean',
    },
    maxReturnObjectCount: {
      value: [1, 2, 3],
      type: 'number',
    },
    enableObjectCenterEstimation: {
      value: [true, false],
      type: 'boolean',
    },
};

function testPositive(orOptionsUnit) {
  let key;
  // eslint-disable-next-line guard-for-in
  for (let k in orOptionsUnit['data']) {
    key = k;
  }
  describe('Object Recognition Test Suite - or - Positive - ' + key, function() {
    // eslint-disable-next-line max-len
    it('Method getObjectRecognitionOptions - ' + key + ' - ' + orOptionsUnit['data'][key], function() {
      return addon.createObjectRecognizer()
      .then(function(objectRecognizer) {
        or = objectRecognizer;
        return or.setObjectRecognitionOptions(orOptionsUnit['data']);
      })
      .then(function() {
        return or.getObjectRecognitionOptions();
      })
      .then(function(options) {
        assert.equal(options[key], orOptionsUnit['data'][key]);
        assert.equal(typeof (options[key]), orOptionsUnit['type']);
      })
      .catch(function(err) {
        console.log(err);
        assert.ok(false);
      });
    });
  });
}

// eslint-disable-next-line guard-for-in
for (let k in orOptionsPositive) {
  for (let i = 0; i < orOptionsPositive[k]['value'].length; i++) {
    let orOptionsUnit = {};
    let orOptionsData = {};
    orOptionsData[k] = orOptionsPositive[k]['value'][i];
    orOptionsUnit['data'] = orOptionsData;
    orOptionsUnit['type'] = orOptionsPositive[k]['type'];
    testPositive(orOptionsUnit);
  }
}

describe('Object Recognition Test Suite - or', function() {
  it('Method getObjectRecognitionOptions - one parameter', function() {
    return addon.createObjectRecognizer()
    .then(function(objectRecognizer) {
      or = objectRecognizer;
      return or.getObjectRecognitionOptions('abc');
    })
    .then(function(options) {
      assert.ok(true);
    })
    .catch(function(err) {
      console.log(err);
      assert.ok(false);
    });
  });
});
