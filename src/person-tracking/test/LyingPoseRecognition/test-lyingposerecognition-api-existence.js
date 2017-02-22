// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
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
describe('LyingPoseRecognition API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let personTrackerConfig = {};
      personTrackerConfig = {
        lying: {
          enable: true,
          maxTrackedPerson: 1,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        obj = personTracker.lyingPoseRecognition;
	done();
      });
    // eslint-disable-next-line
    this.timeout(60000);
    });
    it('pt.LyingPoseRecognition is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.LyingPoseRecognition type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('new pt.LyingPoseRecognition() object has a method .setRecognitionState', function() {
      assert.equal(typeof (obj.setRecognitionState), 'function');
    });

    it('new pt.LyingPoseRecognition() object has a method .getRecognitionState', function() {
      assert.equal(typeof (obj.getRecognitionState), 'function');
    });

    it('new pt.LyingPoseRecognition() object has a method .getCandidatesCount', function() {
      assert.equal(typeof (obj.getCandidatesCount), 'function');
    });

    it('new pt.LyingPoseRecognition() object has a method .getCandidatesData', function() {
      assert.equal(typeof (obj.getCandidatesData), 'function');
    });
  });
});
