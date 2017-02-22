// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global before, describe, it */
/* eslint no-invalid-this: "off" */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

let personTrackerConfig = {};
personTrackerConfig = {
  recognition: {
    enable: true,
  },
};
let obj;
describe.skip('RecognitionInfo API Test', function() {
  before(function(done) {
    pt.createPersonTracker(personTrackerConfig).then((instance) => {
      personTracker = instance;
      personTracker.faceRecognition.recognizeAll().then(
        function(data) {
          obj = data[0];
          done();
        }
      );
    });
    this.timeout(60000);
  });
  describe('API Existance', function() {
    it('pt.RecognitionInfo is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.RecognitionInfo type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('pt.RecognitionInfo() object has the attribute trackID', function() {
      assert.ok(obj.trackID !== undefined);
    });

    it('pt.RecognitionInfo() object has the number attribute trackID', function() {
      assert.equal(typeof (obj.trackID), 'number');
    });

    it('pt.RecognitionInfo() object has the writable attribute trackID', function() {
      let tmp;
      tmp = obj.trackID;
      obj.trackID = NaN;
      assert.notEqual(tmp, obj.trackID);
    });

    it('pt.RecognitionInfo() object has the attribute recognitionID', function() {
      assert.ok(obj.recognitionID !== undefined);
    });

    it('pt.RecognitionInfo() object has the number attribute recognitionID', function() {
      assert.equal(typeof (obj.recognitionID), 'number');
    });

    it('pt.RecognitionInfo() object has the writable attribute recognitionID', function() {
      let tmp;
      tmp = obj.recognitionID;
      obj.recognitionID = NaN;
      assert.notEqual(tmp, obj.recognitionID);
    });

    it('pt.RecognitionInfo() object has the attribute similarityScore', function() {
      assert.ok(obj.similarityScore !== undefined);
    });

    it('pt.RecognitionInfo() object has the number attribute similarityScore', function() {
      assert.equal(typeof (obj.similarityScore), 'number');
    });

    it('pt.RecognitionInfo() object has the writable attribute similarityScore', function() {
      let tmp;
      tmp = obj.similarityScore;
      obj.similarityScore = NaN;
      assert.notEqual(tmp, obj.similarityScore);
    });
  });
});
