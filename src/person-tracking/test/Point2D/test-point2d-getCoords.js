// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Point2D API Test', function() {
  describe('getCoords Test', function() {
    it.skip('no parameter, result type should be array', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let coords = obj.getCoords();
      assert.ok(Array.isArray(coords));
    });

    it.skip('parameter is one string', function() {
      let obj = new pt.Point2D(1.0, 2.0);
      let coords = obj.getCoords('illegal');
      assert.ok(Array.isArray(coords));
    });
  });
});
