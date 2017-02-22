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
describe('PersonTrackingResult API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        skeleton: {
          enable: true,
        },
        tracking: {
          enable: true,
        },
        face: {
          enable: true,
          policy: 'standard',
          useMultiFrame: false,
        },
        pose: {
          enable: true,
        },
        gesture: {
          enable: true,
          enableAllGestures: true,
        },
        expression: {
          enable: true,
          enableAllExpressions: true,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          if (result) {
            obj = result;
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

    it('PersonTrackingResult is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('PersonTrackingResult object has an attribute named persons, type is object', function() {
      assert.equal(typeof (obj.persons), 'object');
    });

    it('persons of PersonTrackingResult is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'persons').writable, false);
    });
  });
});
