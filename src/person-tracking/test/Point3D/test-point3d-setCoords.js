// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Point3D API Test', function() {
  describe('setCoords Test', function() {
    it.skip('parameters are three numbers', function() {
      let obj = new pt.Point3D(1.0, 2.0, 3.0);
      obj.setCoords(3.0, 4.0, 5.0);
      assert.equal(obj.x, 3.0);
      assert.equal(obj.y, 4.0);
      assert.equal(obj.z, 5.0);
    });

    it.skip('parameter is one sequence', function() {
      let obj = new pt.Point3D(1.0, 2.0, 3.0);
      obj.setCoords([3.0, 4.0, 5.0]);
      assert.equal(obj.x, 3.0);
      assert.equal(obj.y, 4.0);
      assert.equal(obj.z, 5.0);
    });
  });
});
