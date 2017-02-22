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
    mode: ['single-recognition', 'tracking', 'localization'],
    framesSkipping: [-1, 0, 60, 2147483647],
    confidenceThreshold: [-0.15, 0, 0.15, 0.75, 1],
    computeEngine: ['CPU', 'GPU'],
    enableSegmentation: [true, false],
    maxReturnObjectCount: [1, 2, 3],
    enableObjectCenterEstimation: [true, false],
};

const orOptionsNegative = {
    mode: [
      'single-recognition111', // invalid value
    ],
    framesSkipping: [
      2147483648,
    ],
    confidenceThreshold: [
      '0.15', // invalid type
    ],
    computeEngine: [
      'CPU1', // invalid value
      'GPU2', // invalid value
    ],
    enableSegmentation: [
      123, // invalid value
    ],
    maxReturnObjectCount: [
      -1, // invalid value
      'string', // invalid type
    ],
    enableObjectCenterEstimation: [
      123, // invalid value
    ],
};

function testPositive(orOptions) {
  let key;
  // eslint-disable-next-line guard-for-in
  for (let k in orOptions) {
    key = k;
  }
  describe('Object Recognition Test Suite - or - Positive - ' + key, function() {
    // eslint-disable-next-line max-len
    it('Method setObjectRecognitionOptions - Positive - ' + key + ' - ' + orOptions[key], function() {
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then(function(objectRecognizer) {
          or = objectRecognizer;
          return or.setObjectRecognitionOptions(orOptions);
        }).then(function() {
          return or.getObjectRecognitionOptions();
        }).then(function(options) {
          assert.equal(options[key], orOptions[key]);
          resolve();
        }).catch(function(err) {
          reject(err);
        });
      });
    });
  });
}

function testcreateObjectRecognizerP(orOptions) {
  let key;
  // eslint-disable-next-line guard-for-in
  for (let k in orOptions) {
    key = k;
  }
  describe('Object Recognition Test Suite - or - Positive - ' + key, function() {
    // eslint-disable-next-line max-len
    it('Method createObjectRecognizer() - Positive - ' + key + ' - ' + orOptions[key], function() {
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer(orOptions).then(function(objectRecognizer) {
          or = objectRecognizer;
          return or.getObjectRecognitionOptions();
        }).then(function(options) {
          assert.equal(options[key], orOptions[key]);
          resolve();
        }).catch(function(err) {
          reject(err);
        });
      });
    });
  });
}

function testNegative(orOptions) {
  let key;
  // eslint-disable-next-line guard-for-in
  for (let k in orOptions) {
    key = k;
  }
  describe('Object Recognition Test Suite - or - Negative - ' + key, function() {
    // eslint-disable-next-line max-len
    it('Method setObjectRecognitionOptions - Negative - ' + key + ' - ' + orOptions[key], function() {
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then(function(objectRecognizer) {
          or = objectRecognizer;
          return or.setObjectRecognitionOptions(orOptions);
        }).then(function() {
          reject('Should reject but promise got resolved');
        }).catch(function(err) {
          resolve();
        });
      });
    });
  });
}

function testcreateObjectRecognizerN(orOptions) {
  let key;
  // eslint-disable-next-line guard-for-in
  for (let k in orOptions) {
    key = k;
  }
  describe('Object Recognition Test Suite - or - Negative - ' + key, function() {
    // eslint-disable-next-line max-len
    it('Method createObjectRecognizer() - Negative - ' + key + ' - ' + orOptions[key], function() {
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer(orOptions).then(function(objectRecognizer) {
        }).then(function() {
          resolve();
        }).catch(function(err) {
          reject(err);
        });
      });
    });
  });
}

// eslint-disable-next-line guard-for-in
for (let k in orOptionsPositive) {
  for (let i = 0; i < orOptionsPositive[k].length; i++) {
    let orOptions = {};
    orOptions[k] = orOptionsPositive[k][i];
    testPositive(orOptions);
    testcreateObjectRecognizerP(orOptions);
  }
}

// eslint-disable-next-line guard-for-in
for (let k in orOptionsNegative) {
  for (let i = 0; i < orOptionsNegative[k].length; i++) {
    let orOptions = {};
    orOptions[k] = orOptionsNegative[k][i];
    testNegative(orOptions);
    testcreateObjectRecognizerN(orOptions);
  }
}
