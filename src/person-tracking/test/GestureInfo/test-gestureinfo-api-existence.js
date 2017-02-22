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
describe('GestureInfo API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        tracking: {
          trackingMode: 'interactive',
        },
        gesture: {
          enable: true,
          enableAllGestures: true,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.gestureInfo) {
            obj = person.gestureInfo;
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

    it('GestureInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('GestureInfo object has an attribute named isPointing, type is boolean', function() {
      assert.equal(typeof (obj.isPointing), 'boolean');
    });

    it('isPointing of GestureInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'isPointing').writable, false);
    });

    it('GestureInfo object has an attribute named pointingInfo, type is object', function() {
      assert.equal(typeof (obj.pointingInfo), 'object');
    });

    it('pointingInfo of GestureInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'pointingInfo').writable, false);
    });
  });
});
