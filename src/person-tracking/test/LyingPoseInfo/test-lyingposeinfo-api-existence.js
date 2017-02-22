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
describe.skip('LyingPoseInfo API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        lying: {
          enable: true,
          maxTrackedPerson: 1,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          if (personTracker.lyingPoseRecognition) {
            obj = personTracker.lyingPoseRecognition.getCandidatesData();
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

    it('LyingPoseInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('LyingPoseInfo object has an attribute named position, type is object', function() {
      assert.equal(typeof (obj.position), 'object');
    });

    it('position of LyingPoseInfo() is writable', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'position').writable, true);
    });

    it('LyingPoseInfo object has an attribute named boundingBox, type is object', function() {
      assert.equal(typeof (obj.boundingBox), 'object');
    });

    it('boundingBox of LyingPoseInfo() is writable', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'boundingBox').writable, true);
    });

    it('LyingPoseInfo object has an attribute named result, type is object', function() {
      assert.equal(typeof (obj.result), 'object');
    });

    it('result of LyingPoseInfo() is writable', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'result').writable, true);
    });

    it('LyingPoseInfo object has an attribute named confidence, type is number', function() {
      assert.equal(typeof (obj.confidence), 'number');
    });

    it('confidence of LyingPoseInfo() is writable', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'confidence').writable, true);
    });
  });
});
