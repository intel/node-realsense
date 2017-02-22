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
  describe.skip('PoseInfo API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        recognition: {
          enable: true,
          policy: 'standard',
          useMultiFrame: false,
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
        personFace: {
          enableHeadPose: true,
          enableFaceLandmarks: true,
          expression: {
            enable: true,
            enableAllExpressions: true,
          },
        },
      };

      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.poseInfo) {
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

    it('PersonInfo object has the attribute poseInfo', function() {
      assert.ok(obj.poseInfo !== undefined);
    });

    it('PersonInfo object has the object attribute poseInfo', function() {
      assert.equal(typeof (obj.poseInfo), 'object');
    });

    it('PersonInfo object has the readonly attribute poseInfo', function() {
      assert.throws(function() {
        obj.poseInfo = null;
      });
    });
  });
});
