// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Range API Test', function() {
  describe('API Existance', function() {
    let obj = new pt.Range(1.0, 2.0);
    it('pt.Range is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.Range type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('pt.Range() object has the attribute begin', function() {
      assert.ok(obj.begin !== undefined);
    });

    it('pt.Range() object has the number attribute begin', function() {
      assert.equal(typeof (obj.begin), 'number');
    });

    it('pt.Range() object has the writable attribute begin', function() {
      let tmp;
      tmp = obj.begin;
      obj.begin = NaN;
      assert.notEqual(tmp, obj.begin);
    });

    it('pt.Range() object has the attribute end', function() {
      assert.ok(obj.end !== undefined);
    });

    it('pt.Range() object has the number attribute end', function() {
      assert.equal(typeof (obj.end), 'number');
    });

    it('pt.Range() object has the writable attribute end', function() {
      let tmp;
      tmp = obj.end;
      obj.end = NaN;
      assert.notEqual(tmp, obj.end);
    });

    it('pt.Range() object has the attribute min', function() {
      assert.ok(obj.min !== undefined);
    });

    it('pt.Range() object has the number attribute min', function() {
      assert.equal(typeof (obj.min), 'number');
    });

    it('pt.Range() object has the writable attribute min', function() {
      let tmp;
      tmp = obj.min;
      obj.min = NaN;
      assert.notEqual(tmp, obj.min);
    });

    it('pt.Range() object has the attribute max', function() {
      assert.ok(obj.max !== undefined);
    });

    it('pt.Range() object has the number attribute max', function() {
      assert.equal(typeof (obj.max), 'number');
    });

    it('pt.Range() object has the writable attribute max', function() {
      let tmp;
      tmp = obj.max;
      obj.max = NaN;
      assert.notEqual(tmp, obj.max);
    });

    it('pt.Range() object has the attribute length', function() {
      assert.ok(obj.length !== undefined);
    });

    it('pt.Range() object has the number attribute length', function() {
      assert.equal(typeof (obj.length), 'number');
    });

    it('pt.Range() object has the readonly attribute length', function() {
      assert.throws(function() {
        obj.length = NaN;
      });
    });

    it('new pt.Range() object has a method .contains', function() {
      assert.equal(typeof (obj.contains), 'function');
    });

    it('new pt.Range() object has a method .properlyContains', function() {
      assert.equal(typeof (obj.properlyContains), 'function');
    });

    it('new pt.Range() object has a method .equal', function() {
      assert.equal(typeof (obj.equal), 'function');
    });

    it('new pt.Range() object has a method .isEmpty', function() {
      assert.equal(typeof (obj.isEmpty), 'function');
    });

    it('new pt.Range() object has a method .isValid', function() {
      assert.equal(typeof (obj.isValid), 'function');
    });
  });
});
