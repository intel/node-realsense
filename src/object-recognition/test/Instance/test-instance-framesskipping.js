// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
// const assert = require('assert');
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

function test(mode, eventName, framesSkipping, expect) {
  describe('Object Recognition Test Suite - framesSkipping - ' + mode, function() {
    it('Run OR by skipping every ' + framesSkipping + ' frames', function() {
      // eslint-disable-next-line no-invalid-this
      this.timeout(50000);
      return new Promise((resolve, reject) => {
        let recCounter = 0;
        let frameCounter = 0;
        function processFrameEvent() {
          frameCounter++;
        }

        function processEvent() {
          recCounter++;
          if (frameCounter !== recCounter * (expect + 1)) {
            reject('OR did not skip every ' + expect + ' frames');
          }

          if (or && recCounter > 2) {
            or.stop().then(function() {
              resolve();
            });
            or.removeAllListeners(eventName);
            or.removeAllListeners('framecaptured');
            or = undefined;
          }
        }

        const options = {
          mode: mode,
          framesSkipping: framesSkipping,
          confidenceThreshold: 0.15,
          computeEngine: 'CPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: false,
        };

        addon.createObjectRecognizer(options).then((objectRecognizer) => {
          or = objectRecognizer;
          or.on(eventName, function(e) {
            processEvent();
          });
          or.on('framecaptured', function(e) {
            processFrameEvent();
          });

          return or.start();
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
}
function testTracking(mode, eventName, framesSkipping, expect) {
  describe('Object Recognition Test Suite - framesSkipping - ' + mode, function() {
    it('Run OR by skipping every ' + framesSkipping + ' frames', function() {
      // eslint-disable-next-line no-invalid-this
      this.timeout(50000);
      return new Promise((resolve, reject) => {
        let recCounter = 0;
        let frameCounter = 0;
        function processFrameEvent() {
          frameCounter++;
        }
        function setTracking(results) {
          let trackingOptions = {
            mode: 'tracking',
            framesSkipping: framesSkipping,
            trackingRois: [],
          };
          let roi = results[0].roi;
          trackingOptions.trackingRois.push(new addon.Rect2D(roi.x, roi.y, roi.width, roi.height));
          or.setObjectRecognitionOptions(trackingOptions).then(() => {
            or.on('tracking', function(e) {
              processEvent();
            });
            or.on('framecaptured', function(e) {
              processFrameEvent();
            });
          });
        }
        function processEvent() {
          recCounter++;
          if (frameCounter !== recCounter * (expect + 1)) {
            reject('OR did not skip every ' + expect + ' frames');
          }

          if (or && recCounter > 2) {
            or.stop().then(function() {
              resolve();
            });
            or.removeAllListeners(eventName);
            or.removeAllListeners('framecaptured');
            or = undefined;
          }
        }

        const options = {
          mode: 'localization',
          framesSkipping: framesSkipping,
          confidenceThreshold: 0.15,
          computeEngine: 'CPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: false,
        };

        addon.createObjectRecognizer(options).then((objectRecognizer) => {
          or = objectRecognizer;
          or.on('localization', function(e) {
            setTracking(e);
          });

          return or.start();
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
}
// test(<mode>, <eventName>, framesSkipping, expectSkipping)
test('single-recognition', 'recognition', 60, 60);
test('single-recognition', 'recognition', -1, 0);
test('localization', 'localization', 60, 60);
test('localization', 'localization', -1, 0);
testTracking('tracking', 'tracking', 60, 60);
