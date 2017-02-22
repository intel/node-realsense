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
describe('FaceRecognition API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        face: {
          enable: true,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          if (personTracker.faceRecognition) {
            obj = personTracker.faceRecognition;
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

    it('FaceRecognition is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('FaceRecognition object has a function named getRegisteredIDs', function() {
      assert.equal(typeof (obj.getRegisteredIDs), 'function');
    });

    it('FaceRecognition object has a function named clearDatabase', function() {
      assert.equal(typeof (obj.clearDatabase), 'function');
    });

    it('FaceRecognition object has a function named exportDatabase', function() {
      assert.equal(typeof (obj.exportDatabase), 'function');
    });

    it('FaceRecognition object has a function named importDatabase', function() {
      assert.equal(typeof (obj.importDatabase), 'function');
    });

    it('FaceRecognition object has a function named recognizeAll', function() {
      assert.equal(typeof (obj.recognizeAll), 'function');
    });

    it('FaceRecognition object has a function named registerPerson', function() {
      assert.equal(typeof (obj.registerPerson), 'function');
    });

    it('FaceRecognition object has a function named unRegisterPerson', function() {
      assert.equal(typeof (obj.unRegisterPerson), 'function');
    });

    it('FaceRecognition object has a function named isPersonRegistered', function() {
      assert.equal(typeof (obj.isPersonRegistered), 'function');
    });

    it('FaceRecognition object has a function named reinforceRegistration', function() {
      assert.equal(typeof (obj.reinforceRegistration), 'function');
    });

    it('FaceRecognition object has a function named recognize', function() {
      assert.equal(typeof (obj.recognize), 'function');
    });

    it('FaceRecognition object has a function named querySimilarityScoreByID', function() {
      assert.equal(typeof (obj.querySimilarityScoreByID), 'function');
    });
  });
});
