// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
/* eslint no-invalid-this: "off" */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');
const utils = require('../../common/tests/testUtils.js');

const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.ObjectRecognizer, EventEmitter);

let or = undefined;

describe('Object Recognition Test Suite - or', function() {
  const orOptions = {
      mode: 'localization',
      confidenceThreshold: 0.15,
      computeEngine: 'CPU',
      enableSegmentation: true,
      maxReturnObjectCount: 3,
      enableObjectCenterEstimation: true,
  };
  let obj;
  let fDone = false;
  before(function(done) {
    this.timeout(60000);
    addon.createObjectRecognizer()
    .then(function(objectRecognizer) {
      or = objectRecognizer;
      or.on('localization', function(evt) {
        or.getFrameData().then(function(data) {
          if (!fDone) {
            obj = data;
            fDone = true;
            done();
          }
        });
      });
      return or.setObjectRecognitionOptions(orOptions);
    })
    .then(function(options) {
      return or.start();
    })
    .then(function() {
    })
    .catch(function(err) {
      assert.ok(false);
    });
  });

  it('FrameData exists and type is correct', function() {
    assert.equal(typeof (obj), 'object');
    assert.equal(utils.isFrameData(obj, 'or'), true);
    assert(obj instanceof addon.FrameData);
    // Note: no need to test more on FrameData interface, since
    //       there is (or will be) a test suite on this FrameData interface
  });
});
