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
describe.skip('ExpressionInfo API Test', function() {
  describe('API Existance', function() {
    before(function(done) {
      let fDone = false;
      let personTrackerConfig = {};
      personTrackerConfig = {
        personFace: {
          expression: {
            enable: true,
            enableAllExpression: true,
          },
        },
      };
      pt.createPersonTracker(personTrackerConfig).then((instance) => {
        personTracker = instance;
        personTracker.on('persontracked', function(result) {
          let person = result.persons[0];
          if (person && person.faceInfo && person.faceInfo.expressionInfo) {
            obj = person.faceInfo.expressionInfo.infoCollection[0];
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

    it('ExpressionInfo is exist and type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('ExpressionInfo object has an attribute named type, type is string', function() {
      assert.equal(typeof (obj.type), 'string');
    });

    it('type of ExpressionInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'type').writable, false);
    });

    it('ExpressionInfo object has an attribute named confidence, type is number', function() {
      assert.equal(typeof (obj.confidence), 'number');
    });

    it('confidence of ExpressionInfo is readonly', function() {
      assert.equal(Object.getOwnPropertyDescriptor(obj, 'confidence').writable, false);
    });
  });
});
