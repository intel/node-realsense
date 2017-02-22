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
describe('PersonFaceInfo API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        personFace: {
          enableFaceLandmarks: true,
          enableHeadPose: true,
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
          if (person && person.faceInfo
            && person.faceInfo.headPose
            // comment out for RC5
            // && person.faceInfo.expressionInfo
            && person.faceInfo.landmarkInfo) {
            obj = person.faceInfo;
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

    it('PersonFaceInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('PersonFaceInfo object has an attribute named landmarkInfo, type is object', function() {
      assert.equal(typeof (obj.landmarkInfo), 'object');
    });

    it('landmarkInfo of PersonFaceInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'landmarkInfo').writable, false);
    });

    it('PersonFaceInfo object has an attribute named headPose, type is object', function() {
      assert.equal(typeof (obj.headPose), 'object');
    });

    it('headPose of PersonFaceInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'headPose').writable, false);
    });

    it.skip('PersonFaceInfo object has expressionInfo attribute, type is object', function() {
      assert.equal(typeof (obj.expressionInfo), 'object');
    });

    it.skip('expressionInfo of PersonFaceInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'expressionInfo').writable, false);
    });
  });
});
