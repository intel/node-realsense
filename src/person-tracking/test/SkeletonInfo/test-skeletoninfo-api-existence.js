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
describe('SkeletonInfo API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        tracking: {
          trackingMode: 'interactive',
        },
        skeleton: {
          enable: true,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.skeletonInfo) {
            obj = person.skeletonInfo;
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
      this.timeout(5000);
      return personTracker.stop();
    });

    it('SkeletonInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('SkeletonInfo object has an attribute named skeletonJoints, type is object', function() {
      assert.equal(typeof (obj.skeletonJoints), 'object');
    });

    it('skeletonJoints of SkeletonInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'skeletonJoints').writable, false);
    });
  });
});
