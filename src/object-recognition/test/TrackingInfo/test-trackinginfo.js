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

let obj;

describe('TrackingInfo Test Suite - Run', function() {
  before(function() {
    this.timeout(50000);
    let or = undefined;
    function setTracking(results) {
      let trackingOptions = {
        mode: 'tracking',
        trackingRois: [],
      };
      let roi = results[0].roi;
      trackingOptions.trackingRois.push(new addon.Rect2D(roi.x, roi.y, roi.width, roi.height));
      or.setObjectRecognitionOptions(trackingOptions);
    }
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
        or.on('localization', function(d) {
          setTracking(d);
        });
        or.on('tracking', function(data) {
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

  it('TrackingInfo object has an attribute named roi, type is object', function() {
    assert.equal(utils.isRect2DObject(obj.roi), true);
  });

  it('roi of TrackingInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'roi').writable, false);
  });

  it('TrackingInfo object has an attribute named objectCenter, type is object', function() {
    assert.equal(utils.isPoint3DObject(obj.objectCenter), true);
  });

  it('objectCenter of TrackingInfo is readonly', function() {
    assert.equal(Object.getOwnPropertyDescriptor(obj, 'objectCenter').writable, false);
  });
});
