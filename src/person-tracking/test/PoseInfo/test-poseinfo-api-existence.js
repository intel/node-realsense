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
describe('PoseInfo API Test', function() {
  describe.skip('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        pose: {
          enable: true,
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.poseInfo) {
            obj = person.poseInfo;
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

    it('PoseInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('PoseInfo object has an attribute named position, type is string', function() {
      assert.equal(typeof (obj.position), 'string');
    });

    it('position of PoseInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'position').writable, false);
    });

    it('PoseInfo object has an attribute named confidence, type is number', function() {
      assert.equal(typeof (obj.confidence), 'number');
    });

    it('confidence of PoseInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'confidence').writable, false);
    });
  });
});
