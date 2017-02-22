// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global before, describe, it */
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
describe('PersonTracker API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      // eslint-disable-next-line no-invalid-this
      this.timeout(30000);
      let personTrackerConfig = {};
      personTrackerConfig = {
        lying: {
          enable: true,
          maxTrackedPerson: 1,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
	obj = personTracker;
        done();
      });
    });
    it('PersonTracker is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('PersonTracker type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('PersonTracker object has the attribute state', function() {
      assert.ok(obj.state !== undefined);
    });

    it('PersonTracker object has the string attribute state', function() {
      assert.equal(typeof (obj.state), 'string');
    });

    it('PersonTracker object has the readonly attribute state', function() {
      assert.throws(function() {
        obj.state = null;
      });
    });

    it('new PersonTracker object has a method .getPersonTrackerOptions', function() {
      assert.equal(typeof (obj.getPersonTrackerOptions), 'function');
    });

    it('new PersonTracker object has a method .setPersonTrackerOptions', function() {
      assert.equal(typeof (obj.setPersonTrackerOptions), 'function');
    });

    it('new PersonTracker object has a method .start', function() {
      assert.equal(typeof (obj.start), 'function');
    });

    it('new PersonTracker object has a method .stop', function() {
      assert.equal(typeof (obj.stop), 'function');
    });

    it('new PersonTracker object has a method .pause', function() {
      assert.equal(typeof (obj.pause), 'function');
    });

    it('new PersonTracker object has a method .resume', function() {
      assert.equal(typeof (obj.resume), 'function');
    });

    it('new PersonTracker object has a method .reset', function() {
      assert.equal(typeof (obj.reset), 'function');
    });

    it('PersonTracker object has the attribute faceRecognition', function() {
      assert.ok(obj.faceRecognition !== undefined);
    });

    it('PersonTracker object has the object attribute faceRecognition', function() {
      assert.equal(typeof (obj.faceRecognition), 'object');
    });

    it('PersonTracker object has the readonly attribute faceRecognition', function() {
      assert.throws(function() {
        obj.faceRecognition = null;
      });
    });

    it('PersonTracker object has the attribute lyingPoseRecognition', function() {
      assert.ok(obj.lyingPoseRecognition !== undefined);
    });

    it('PersonTracker object has the object attribute lyingPoseRecognition', function() {
      assert.equal(typeof (obj.lyingPoseRecognition), 'object');
    });

    it('PersonTracker object has the readonly attribute lyingPoseRecognition', function() {
      assert.throws(function() {
        obj.lyingPoseRecognition = null;
      });
    });

    it('PersonTracker object has the attribute personTracking', function() {
      assert.ok(obj.personTracking !== undefined);
    });

    it('PersonTracker object has the object attribute personTracking', function() {
      assert.equal(typeof (obj.personTracking), 'object');
    });

    it('PersonTracker object has the readonly attribute personTracking', function() {
      assert.throws(function() {
        obj.personTracking = null;
      });
    });
  });
});
