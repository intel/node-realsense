// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
/* eslint no-invalid-this: "off" */
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
  enableObjectCenterEstimation: [false],
  mode: ['single-recognition', 'tracking', 'localization'],
  confidenceThreshold: [0.15, 0.75],
  computeEngine: ['CPU', 'GPU'],
  enableSegmentation: [true, false],
  maxReturnObjectCount: [1, 2, 3],
};

const orOptionsNegative = {
  mode: [
    'single-recognition111', // invalid value
  ],
  confidenceThreshold: [
    -0.15,
    0,
    1,
    '0.15', // invalid type
  ],
  computeEngine: [
    'CPU1', // invalid value
    'GPU2', // invalid value
  ],
  // enableSegmentation: [
  //   123, // invalid value
  // ],
  maxReturnObjectCount: [
    -1, // invalid value
    'string', // invalid type
  ],
  enableObjectCenterEstimation: [
    true,
    123, // invalid value
  ],
};

function testPositive(orOptions) {
  let key;
  let mode = 'single-recognition';
  // eslint-disable-next-line guard-for-in
  for (let k in orOptions) {
    key = k;
    if(k === 'mode') {
      mode = orOptions[k];
    }
  }
  describe('Object Recognition Test Suite - or - Positive - ' + key, function() {
    after(function() {
      this.timeout(15000);
      return new Promise((resolve, reject) => {
        or.stop().then(function() {
          console.log('Stop Camera');
          resolve();
        }).catch(() => {
          reject();
        });
      });
    });

    it('Method start() - Positive - ' + mode + ' - ' + key + ' - ' + orOptions[key], function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then((objectRecognizer) => {
          or = objectRecognizer;
          return or.setObjectRecognitionOptions(orOptions);
        }).then(() => {
          return or.start();
        }).then(() => {
          resolve();
        }).catch((e) => {
          reject(e);
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
    after(function() {
      this.timeout(15000);
      return new Promise((resolve, reject) => {
        if(or.state === 'running') {
          or.stop().then(function() {
            console.log('Stop Camera');
            resolve();
          }).catch(() => {
            reject();
          });
        }
        resolve();
      });
    });

    it('Method start() - Negative - ' + key + ' - ' + orOptions[key], function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then((objectRecognizer) => {
          or = objectRecognizer;
          return or.setObjectRecognitionOptions(orOptions);
        }).then(() => {
          return or.start();
        }).then(() => {
          reject('should reject but promise got resolve');
        }).catch((e) => {
          resolve();
        });
      });
    });
  });
}

testPositive({mode: 'localization', enableObjectCenterEstimation: true});
testPositive({mode: 'localization', enableObjectCenterEstimation: false});
// eslint-disable-next-line guard-for-in
for (let k in orOptionsPositive) {
  for (let i = 0; i < orOptionsPositive[k].length; i++) {
    let orOptions = {};
    orOptions[k] = orOptionsPositive[k][i];
    testPositive(orOptions);
  }
}

// eslint-disable-next-line guard-for-in
for (let k in orOptionsNegative) {
  for (let i = 0; i < orOptionsNegative[k].length; i++) {
    let orOptions = {};
    orOptions[k] = orOptionsNegative[k][i];
    testNegative(orOptions);
  }
}
