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
describe('PersonInfo API Test', function() {
  describe('PersonFaceInfo API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        expression: {
          enable: true,
          enableAllExpressions: true,
        },
        personFace: {
          enableFaceLandmarks: true,
          enableHeadPose: true,
        },
        pose: {
          enable: true,
        },
        skeleton: {
          enable: true,
        },
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
          if (person && person.faceInfo) {
            obj = person;
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

    it('PersonInfo object has the attribute personFaceInfo', function() {
      assert.ok(obj.faceInfo !== undefined);
    });

    it('PersonInfo object has the object attribute personFaceInfo', function() {
      assert.equal(typeof (obj.faceInfo), 'object');
    });

    it('PersonInfo object has the readonly attribute personFaceInfo', function() {
      assert.throws(function() {
        obj.faceInfo = null;
      });
    });
  });
});
