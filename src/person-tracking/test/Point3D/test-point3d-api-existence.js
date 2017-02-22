// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

let obj;
describe('Point3D API Test', function() {
  describe('API Existance', function() {
    obj = new pt.Point3D(1.0, 2.0, 3.0);

    it('pt.Point3D is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.Point3D type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('pt.Point3D() object has the attribute x', function() {
      assert.ok(obj.x !== undefined);
    });

    it('pt.Point3D() object has the number attribute x', function() {
      assert.equal(typeof (obj.x), 'number');
    });

    it('pt.Point3D() object has the writable attribute x', function() {
      let tmp;
      tmp = obj.x;
      obj.x = NaN;
      assert.notEqual(tmp, obj.x);
    });

    it('pt.Point3D() object has the attribute y', function() {
      assert.ok(obj.y !== undefined);
    });

    it('pt.Point3D() object has the number attribute y', function() {
      assert.equal(typeof (obj.y), 'number');
    });

    it('pt.Point3D() object has the writable attribute y', function() {
      let tmp;
      tmp = obj.y;
      obj.y = NaN;
      assert.notEqual(tmp, obj.y);
    });

    it('pt.Point3D() object has the attribute z', function() {
      assert.ok(obj.z !== undefined);
    });

    it('pt.Point3D() object has the number attribute z', function() {
      assert.equal(typeof (obj.z), 'number');
    });

    it('pt.Point3D() object has the writable attribute z', function() {
      let tmp;
      tmp = obj.z;
      obj.z = NaN;
      assert.notEqual(tmp, obj.z);
    });

    it('new pt.Point3D() object has a method .setCoords', function() {
      assert.equal(typeof (obj.setCoords), 'function');
    });

    it('new pt.Point3D() object has a method .getCoords', function() {
      assert.equal(typeof (obj.getCoords), 'function');
    });

    it('new pt.Point3D() object has a method .equal', function() {
      assert.equal(typeof (obj.equal), 'function');
    });

    it.skip('new pt.Point3D() object has a method .distance', function() {
      assert.equal(typeof (obj.distance), 'function');
    });
  });
});
