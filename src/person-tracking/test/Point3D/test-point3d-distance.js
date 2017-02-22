// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Point3D API Test', function() {
  describe('distance Test', function() {
    it.skip('parameters are two point3D objects', function() {
      let obj = new pt.Point3D(1.0, 2.0, 3.0);
      let objTest = new pt.Point3D(1.0, 2.0, 4.0);
      let result = obj.distance(obj, objTest);
      assert.equal(result, 1.0);
    });
  });
});
