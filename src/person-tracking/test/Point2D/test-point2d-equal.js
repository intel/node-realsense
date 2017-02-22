// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Point2D API Test', function() {
  describe('equal Test', function() {
    it.skip('arameter is one point2D object', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let objTest = new pt.Point2D(1.0, 2.0);
      let result = obj.equal(objTest);
      assert.ok(result);
    });

    it.skip('parameters are two numbers', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let result = obj.equal(1.0, 2.0);
      assert.ok(result);
    });

    it.skip('(1.0, 2.0) is not equal (3.0, 4.0)', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let result = obj.equal(3.0, 4.0);
      assert.ok(!result);
    });

    it.skip('(1.0, 2.0) is not equal point(3.0, 4.0)', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let objTest = new pt.Point2D(3.0, 4.0);
      let result = obj.equal(objTest);
      assert.ok(!result);
    });
  });
});
