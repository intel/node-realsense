// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global after, before, describe, it */
/* eslint no-invalid-this: "off" */
'use strict';
const emitter = require('events').EventEmitter;
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(pt.PersonTracker, emitter);

let obj;
let personTracker;
describe('MaskInfo API Test', function() {
  describe.skip('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        tracking: {
          enableSegmentation: true,
          trackingMode: 'interactive',
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.trackInfo.segmentationMask) {
            obj = person.trackInfo.segmentationMask;
            if (!fDone) {
              fDone = true;
              done();
            }
          }
        });
        personTracker.start();
      });
      // eslint-disable-next-line
      this.timeout(60000);
    });
    after(function() {
      return personTracker.stop();
    });

    it('MaskInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('MaskInfo object has an attribute named width, type is number', function() {
      assert.equal(typeof (obj.width), 'number');
    });

    it('width of MaskInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'width').writable, false);
    });

    it('MaskInfo object has an attribute named height, type is number', function() {
      assert.equal(typeof (obj.height), 'number');
    });

    it('height of MaskInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'height').writable, false);
    });

    it('MaskInfo object has an attribute named maskData, type is object', function() {
      assert.equal(typeof (obj.maskData), 'object');
    });

    it('maskData of MaskInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'maskData').writable, false);
    });
  });
});
