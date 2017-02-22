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
describe('TrackingInfo API Test', function() {
  describe.skip('BlobMask API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        tracking: {
          enableBlob: true,
          enableHeadPose: true,
          enableSegmentation: true,
          enableHeadBoundingBox: true,
          enableFaceLandmarks: true,
          enablePersonOrientation: true,
          trackingMode: 'interactive',
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.trackInfo.blobMask) {
            obj = person.trackInfo;
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


    it('pt.TrackingInfo() object has the attribute blobMask', function() {
      assert.ok(obj.blobMask !== undefined);
    });

    it('pt.TrackingInfo() object has the object attribute blobMask', function() {
      assert.equal(typeof (obj.blobMask), 'object');
    });

    it('pt.TrackingInfo() object has the readonly attribute blobMask', function() {
      assert.throws(function() {
        obj.blobMask = null;
      });
    });
  });
});
