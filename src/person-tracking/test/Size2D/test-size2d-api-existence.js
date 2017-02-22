// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Size2D API Test', function() {
  describe('API Existance', function() {
    let obj = new pt.Size2D(1.0, 2.0);
    it('pt.Size2D is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.Size2D type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('pt.Size2D() object has the attribute width', function() {
      assert.ok(obj.width !== undefined);
    });

    it('pt.Size2D() object has the number attribute width', function() {
      assert.equal(typeof (obj.width), 'number');
    });

    it('pt.Size2D() object has the writable attribute width', function() {
      let tmp;
      tmp = obj.width;
      obj.width = NaN;
      assert.notEqual(tmp, obj.width);
    });

    it('pt.Size2D() object has the attribute height', function() {
      assert.ok(obj.height !== undefined);
    });

    it('pt.Size2D() object has the number attribute height', function() {
      assert.equal(typeof (obj.height), 'number');
    });

    it('pt.Size2D() object has the writable attribute height', function() {
      let tmp;
      tmp = obj.height;
      obj.height = NaN;
      assert.notEqual(tmp, obj.height);
    });

    it('pt.Size2D() object has the attribute cx', function() {
      assert.ok(obj.cx !== undefined);
    });

    it('pt.Size2D() object has the number attribute cx', function() {
      assert.equal(typeof (obj.cx), 'number');
    });

    it('pt.Size2D() object has the writable attribute cx', function() {
      let tmp;
      tmp = obj.cx;
      obj.cx = NaN;
      assert.notEqual(tmp, obj.cx);
    });

    it('pt.Size2D() object has the attribute cy', function() {
      assert.ok(obj.cy !== undefined);
    });

    it('pt.Size2D() object has the number attribute cy', function() {
      assert.equal(typeof (obj.cy), 'number');
    });

    it('pt.Size2D() object has the writable attribute cy', function() {
      let tmp;
      tmp = obj.cy;
      obj.cy = NaN;
      assert.notEqual(tmp, obj.cy);
    });

    it('new pt.Size2D() object has a method .equal', function() {
      assert.equal(typeof (obj.equal), 'function');
    });

    it('new pt.Size2D() object has a method .isEmpty', function() {
      assert.equal(typeof (obj.isEmpty), 'function');
    });

    it('new pt.Size2D() object has a method .isValid', function() {
      assert.equal(typeof (obj.isValid), 'function');
    });
  });
});
