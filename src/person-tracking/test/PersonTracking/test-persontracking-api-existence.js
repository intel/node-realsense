// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('PersonTracking API Test', function() {
  describe('API Existance', function() {
    let obj;
    before(function(done) {
      // eslint-disable-next-line no-invalid-this
      this.timeout(10000);
      pt.createPersonTracker().then((instance) => {
        obj = instance.personTracking;
        done();
      });
    });
    it('pt.PersonTracking is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.PersonTracking type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('new PersonTracking object has a method .startTrackingPerson', function() {
      assert.equal(typeof (obj.startTrackingPerson), 'function');
    });

    it('new PersonTracking object has a method .stopTrackingPerson', function() {
      assert.equal(typeof (obj.stopTrackingPerson), 'function');
    });
  });
});
