// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Point3D API Test', function() {
  describe('Constructor Test', function() {
    it('parameter is null', function() {
      let obj = new pt.Point3D();
      assert.ok(typeof (obj) !== 'undefined');
      assert.ok(typeof (obj.x) === 'number');
      assert.ok(typeof (obj.y) === 'number');
      assert.ok(typeof (obj.z) === 'number');
    });

    it('parameters are three numbers', function() {
      let obj = new pt.Point3D(1.0, 2.0, 3.0);
      assert.ok(typeof (obj) !== 'undefined');
      assert.ok(typeof (obj.x) === 'number');
      assert.ok(typeof (obj.y) === 'number');
      assert.ok(typeof (obj.z) === 'number');
    });

    it('parameter is one sequence', function() {
      let obj = new pt.Point3D([1.0, 2.0, 3.0]);
      assert.ok(typeof (obj) !== 'undefined');
      assert.ok(typeof (obj.x) === 'number');
      assert.ok(typeof (obj.y) === 'number');
      assert.ok(typeof (obj.z) === 'number');
    });

    it('Illegal: parameter is one sequence of two numbers', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D([1.0, 2.0]);
      });
    });

    it('Illegal: parameter is one sequence of four numbers', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D([1.0, 2.0, 3.0, 4.0]);
      });
    });

    it.skip('Illegal: parameter is one number', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D(1.0);
      });
    });

    it('Illegal: parameters are four numbers', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D(1.0, 2.0, 3.0, 4.0);
      });
    });

    it('Illegal: parameter is one string', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D('abc');
      });
    });

    it('Illegal: parameters are one string and two numbers', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D('abc', 1.0, 2.0);
      });
    });

    it('Illegal: parameter is one sequence with one string', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D(['abc']);
      });
    });

    it('Illegal: parameters are two sequences', function() {
      assert.throws(function() {
        // eslint-disable-next-line
        let obj = new pt.Point3D([1.0, 2.0], [3.0, 4.0]);
      });
    });
  });
});
