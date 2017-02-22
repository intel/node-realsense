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
let previousMode = undefined;
const orOptions = {
    mode: 'single-recognition-1111',
};

describe('Object Recognition Test Suite', function() {
  // eslint-disable-next-line max-len
  it('Method setObjectRecognitionOptions will not take in any invalid values', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then(function(objectRecognizer) {
        or = objectRecognizer;
        return or.getObjectRecognitionOptions();
      }).then(function(options) {
        previousMode = options['mode'];
        return or.setObjectRecognitionOptions(orOptions);
      }).then(function() {
        reject('Should reject promise when setting invalid options');
      }).catch(function(err) {
        return; // Absorb the expected exception
      }).then(function() {
        return or.getObjectRecognitionOptions();
      }).then(function(options) {
        assert.equal(options['mode'], previousMode);
        resolve();
      }).catch(function(err) {
        reject(err);
      });
    });
  });
});
