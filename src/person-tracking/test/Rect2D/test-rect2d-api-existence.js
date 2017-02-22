// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global describe, it */
'use strict';
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

describe('Rect2D API Test', function() {
  describe('API Existance', function() {
    let obj = new pt.Rect2D(1.0, 2.0, 3.0, 4.0);
    it('pt.Rect2D is exist', function() {
      assert.ok(typeof (obj) !== 'undefined');
    });

    it('pt.Rect2D type is correct', function() {
      assert.equal(typeof (obj), 'object');
    });

    it('pt.Rect2D() object has the attribute x', function() {
      assert.ok(obj.x !== undefined);
    });

    it('pt.Rect2D() object has the number attribute x', function() {
      assert.equal(typeof (obj.x), 'number');
    });

    it('pt.Rect2D() object has the writable attribute x', function() {
      let tmp;
      tmp = obj.x;
      obj.x = NaN;
      assert.notEqual(tmp, obj.x);
    });

    it('pt.Rect2D() object has the attribute y', function() {
      assert.ok(obj.y !== undefined);
    });

    it('pt.Rect2D() object has the number attribute y', function() {
      assert.equal(typeof (obj.y), 'number');
    });

    it('pt.Rect2D() object has the writable attribute y', function() {
      let tmp;
      tmp = obj.y;
      obj.y = NaN;
      assert.notEqual(tmp, obj.y);
    });

    it('pt.Rect2D() object has the attribute width', function() {
      assert.ok(obj.width !== undefined);
    });

    it('pt.Rect2D() object has the number attribute width', function() {
      assert.equal(typeof (obj.width), 'number');
    });

    it('pt.Rect2D() object has the writable attribute width', function() {
      let tmp;
      tmp = obj.width;
      obj.width = NaN;
      assert.notEqual(tmp, obj.width);
    });

    it('pt.Rect2D() object has the attribute height', function() {
      assert.ok(obj.height !== undefined);
    });

    it('pt.Rect2D() object has the number attribute height', function() {
      assert.equal(typeof (obj.height), 'number');
    });

    it('pt.Rect2D() object has the writable attribute height', function() {
      let tmp;
      tmp = obj.height;
      obj.height = NaN;
      assert.notEqual(tmp, obj.height);
    });

    it('pt.Rect2D() object has the attribute diagonalLength', function() {
      assert.ok(obj.diagonalLength !== undefined);
    });

    it('pt.Rect2D() object has the number attribute diagonalLength', function() {
      assert.equal(typeof (obj.diagonalLength), 'number');
    });

    it('pt.Rect2D() object has the readonly attribute diagonalLength', function() {
      assert.throws(function() {
        obj.diagonalLength = NaN;
      });
    });

    it('pt.Rect2D() object has the attribute size', function() {
      assert.ok(obj.size !== undefined);
    });

    it('pt.Rect2D() object has the object attribute size', function() {
      assert.equal(typeof (obj.size), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute size', function() {
      let tmp;
      tmp = obj.size;
      obj.size = null;
      assert.notEqual(tmp, obj.size);
    });

    it('pt.Rect2D() object has the attribute topLeft', function() {
      assert.ok(obj.topLeft !== undefined);
    });

    it('pt.Rect2D() object has the object attribute topLeft', function() {
      assert.equal(typeof (obj.topLeft), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute topLeft', function() {
      let tmp;
      tmp = obj.topLeft;
      obj.topLeft = null;
      assert.notEqual(tmp, obj.topLeft);
    });

    it('pt.Rect2D() object has the attribute topRight', function() {
      assert.ok(obj.topRight !== undefined);
    });

    it('pt.Rect2D() object has the object attribute topRight', function() {
      assert.equal(typeof (obj.topRight), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute topRight', function() {
      let tmp;
      tmp = obj.topRight;
      obj.topRight = null;
      assert.notEqual(tmp, obj.topRight);
    });

    it('pt.Rect2D() object has the attribute bottomLeft', function() {
      assert.ok(obj.bottomLeft !== undefined);
    });

    it('pt.Rect2D() object has the object attribute bottomLeft', function() {
      assert.equal(typeof (obj.bottomLeft), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute bottomLeft', function() {
      let tmp;
      tmp = obj.bottomLeft;
      obj.bottomLeft = null;
      assert.notEqual(tmp, obj.bottomLeft);
    });

    it('pt.Rect2D() object has the attribute bottomRight', function() {
      assert.ok(obj.bottomRight !== undefined);
    });

    it('pt.Rect2D() object has the object attribute bottomRight', function() {
      assert.equal(typeof (obj.bottomRight), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute bottomRight', function() {
      let tmp;
      tmp = obj.bottomRight;
      obj.bottomRight = null;
      assert.notEqual(tmp, obj.bottomRight);
    });

    it('pt.Rect2D() object has the attribute center', function() {
      assert.ok(obj.center !== undefined);
    });

    it('pt.Rect2D() object has the object attribute center', function() {
      assert.equal(typeof (obj.center), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute center', function() {
      let tmp;
      tmp = obj.center;
      obj.center = null;
      assert.notEqual(tmp, obj.center);
    });

    it('pt.Rect2D() object has the attribute topCenter', function() {
      assert.ok(obj.topCenter !== undefined);
    });

    it('pt.Rect2D() object has the object attribute topCenter', function() {
      assert.equal(typeof (obj.topCenter), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute topCenter', function() {
      let tmp;
      tmp = obj.topCenter;
      obj.topCenter = null;
      assert.notEqual(tmp, obj.topCenter);
    });

    it('pt.Rect2D() object has the attribute bottomCenter', function() {
      assert.ok(obj.bottomCenter !== undefined);
    });

    it('pt.Rect2D() object has the object attribute bottomCenter', function() {
      assert.equal(typeof (obj.bottomCenter), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute bottomCenter', function() {
      let tmp;
      tmp = obj.bottomCenter;
      obj.bottomCenter = null;
      assert.notEqual(tmp, obj.bottomCenter);
    });

    it('pt.Rect2D() object has the attribute leftCenter', function() {
      assert.ok(obj.leftCenter !== undefined);
    });

    it('pt.Rect2D() object has the object attribute leftCenter', function() {
      assert.equal(typeof (obj.leftCenter), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute leftCenter', function() {
      let tmp;
      tmp = obj.leftCenter;
      obj.leftCenter = null;
      assert.notEqual(tmp, obj.leftCenter);
    });

    it('pt.Rect2D() object has the attribute rightCenter', function() {
      assert.ok(obj.rightCenter !== undefined);
    });

    it('pt.Rect2D() object has the object attribute rightCenter', function() {
      assert.equal(typeof (obj.rightCenter), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute rightCenter', function() {
      let tmp;
      tmp = obj.rightCenter;
      obj.rightCenter = null;
      assert.notEqual(tmp, obj.rightCenter);
    });

    it('pt.Rect2D() object has the attribute top', function() {
      assert.ok(obj.top !== undefined);
    });

    it('pt.Rect2D() object has the number attribute top', function() {
      assert.equal(typeof (obj.top), 'number');
    });

    it.skip('pt.Rect2D() object has the writable attribute top', function() {
      let tmp;
      tmp = obj.top;
      obj.top = NaN;
      assert.notEqual(tmp, obj.top);
    });

    it('pt.Rect2D() object has the attribute left', function() {
      assert.ok(obj.left !== undefined);
    });

    it('pt.Rect2D() object has the number attribute left', function() {
      assert.equal(typeof (obj.left), 'number');
    });

    it.skip('pt.Rect2D() object has the writable attribute left', function() {
      let tmp;
      tmp = obj.left;
      obj.left = NaN;
      assert.notEqual(tmp, obj.left);
    });

    it('pt.Rect2D() object has the attribute bottom', function() {
      assert.ok(obj.bottom !== undefined);
    });

    it('pt.Rect2D() object has the number attribute bottom', function() {
      assert.equal(typeof (obj.bottom), 'number');
    });

    it.skip('pt.Rect2D() object has the writable attribute bottom', function() {
      let tmp;
      tmp = obj.bottom;
      obj.bottom = NaN;
      assert.notEqual(tmp, obj.bottom);
    });

    it('pt.Rect2D() object has the attribute right', function() {
      assert.ok(obj.right !== undefined);
    });

    it('pt.Rect2D() object has the number attribute right', function() {
      assert.equal(typeof (obj.right), 'number');
    });

    it.skip('pt.Rect2D() object has the writable attribute right', function() {
      let tmp;
      tmp = obj.right;
      obj.right = NaN;
      assert.notEqual(tmp, obj.right);
    });

    it('pt.Rect2D() object has the attribute horzRange', function() {
      assert.ok(obj.horzRange !== undefined);
    });

    it('pt.Rect2D() object has the object attribute horzRange', function() {
      assert.equal(typeof (obj.horzRange), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute horzRange', function() {
      let tmp;
      tmp = obj.horzRange;
      obj.horzRange = null;
      assert.notEqual(tmp, obj.horzRange);
    });

    it('pt.Rect2D() object has the attribute vertRange', function() {
      assert.ok(obj.vertRange !== undefined);
    });

    it('pt.Rect2D() object has the object attribute vertRange', function() {
      assert.equal(typeof (obj.vertRange), 'object');
    });

    it.skip('pt.Rect2D() object has the writable attribute vertRange', function() {
      let tmp;
      tmp = obj.vertRange;
      obj.vertRange = null;
      assert.notEqual(tmp, obj.vertRange);
    });

    it('new pt.Rect2D() object has a method .equal', function() {
      assert.equal(typeof (obj.equal), 'function');
    });

    it('new pt.Rect2D() object has a method .equalCoord', function() {
      assert.equal(typeof (obj.equalCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .isEmpty', function() {
      assert.equal(typeof (obj.isEmpty), 'function');
    });

    it('new pt.Rect2D() object has a method .isValid', function() {
      assert.equal(typeof (obj.isValid), 'function');
    });

    it('new pt.Rect2D() object has a method .adjust', function() {
      assert.equal(typeof (obj.adjust), 'function');
    });

    it('new pt.Rect2D() object has a method .expand', function() {
      assert.equal(typeof (obj.expand), 'function');
    });

    it('new pt.Rect2D() object has a method .shrink', function() {
      assert.equal(typeof (obj.shrink), 'function');
    });

    it('new pt.Rect2D() object has a method .move', function() {
      assert.equal(typeof (obj.move), 'function');
    });

    it('new pt.Rect2D() object has a method .moveTopTo', function() {
      assert.equal(typeof (obj.moveTopTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveLeftTo', function() {
      assert.equal(typeof (obj.moveLeftTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveRightTo', function() {
      assert.equal(typeof (obj.moveRightTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveBottomTo', function() {
      assert.equal(typeof (obj.moveBottomTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveTopLeftTo', function() {
      assert.equal(typeof (obj.moveTopLeftTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveTopRightTo', function() {
      assert.equal(typeof (obj.moveTopRightTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveBottomRightTo', function() {
      assert.equal(typeof (obj.moveBottomRightTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveBottomLeftTo', function() {
      assert.equal(typeof (obj.moveBottomLeftTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveCenterTo', function() {
      assert.equal(typeof (obj.moveCenterTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveLeftCenterTo', function() {
      assert.equal(typeof (obj.moveLeftCenterTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveTopCenterTo', function() {
      assert.equal(typeof (obj.moveTopCenterTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveRightCenterTo', function() {
      assert.equal(typeof (obj.moveRightCenterTo), 'function');
    });

    it('new pt.Rect2D() object has a method .moveBottomCenterTo', function() {
      assert.equal(typeof (obj.moveBottomCenterTo), 'function');
    });

    it('new pt.Rect2D() object has a method .normalize', function() {
      assert.equal(typeof (obj.normalize), 'function');
    });

    it('new pt.Rect2D() object has a method .setCoords', function() {
      assert.equal(typeof (obj.setCoords), 'function');
    });

    it('new pt.Rect2D() object has a method .getCoords', function() {
      assert.equal(typeof (obj.getCoords), 'function');
    });

    it('new pt.Rect2D() object has a method .onEdge', function() {
      assert.equal(typeof (obj.onEdge), 'function');
    });

    it('new pt.Rect2D() object has a method .contains', function() {
      assert.equal(typeof (obj.contains), 'function');
    });

    it('new pt.Rect2D() object has a method .containsCoord', function() {
      assert.equal(typeof (obj.containsCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .properlyContains', function() {
      assert.equal(typeof (obj.properlyContains), 'function');
    });

    it('new pt.Rect2D() object has a method .properlyContainsCoord', function() {
      assert.equal(typeof (obj.properlyContainsCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .intersects', function() {
      assert.equal(typeof (obj.intersects), 'function');
    });

    it('new pt.Rect2D() object has a method .intersectsCoord', function() {
      assert.equal(typeof (obj.intersectsCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .unite', function() {
      assert.equal(typeof (obj.unite), 'function');
    });

    it('new pt.Rect2D() object has a method .uniteCoord', function() {
      assert.equal(typeof (obj.uniteCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .split', function() {
      assert.equal(typeof (obj.split), 'function');
    });

    it('new pt.Rect2D() object has a method .splitX', function() {
      assert.equal(typeof (obj.splitX), 'function');
    });

    it('new pt.Rect2D() object has a method .splitY', function() {
      assert.equal(typeof (obj.splitY), 'function');
    });

    it('new pt.Rect2D() object has a method .united', function() {
      assert.equal(typeof (obj.united), 'function');
    });

    it('new pt.Rect2D() object has a method .unitedCoord', function() {
      assert.equal(typeof (obj.unitedCoord), 'function');
    });

    it('new pt.Rect2D() object has a method .intersected', function() {
      assert.equal(typeof (obj.intersected), 'function');
    });

    it('new pt.Rect2D() object has a method .intersectedCoord', function() {
      assert.equal(typeof (obj.intersectedCoord), 'function');
    });
  });
});
