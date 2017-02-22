// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
var assert = require('assert');
var addon = require('bindings')('geometry');
var Rect2D = addon.Rect2D;
var Point2D = addon.Point2D;
var Range = addon.Range;
var Size2D = addon.Size2D;

describe('Geometry Rect2D', function() {
  it('Existence of Rect2D', done => {
    assert.equal(typeof Rect2D, 'function');
    assert.equal(typeof Rect2D.prototype, 'object');
    assert.equal(typeof Rect2D.prototype.equal, 'function');
    assert.equal(typeof Rect2D.prototype.isEmpty, 'function');
    assert.equal(typeof Rect2D.prototype.isValid, 'function');
    assert.equal(typeof Rect2D.prototype.adjust, 'function');
    assert.equal(typeof Rect2D.prototype.expand, 'function');
    assert.equal(typeof Rect2D.prototype.shrink, 'function');
    assert.equal(typeof Rect2D.prototype.move, 'function');
    assert.equal(typeof Rect2D.prototype.moveTopLeftTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveTopRightTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveBottomRightTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveBottomLeftTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveCenterTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveLeftCenterTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveTopCenterTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveRightCenterTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveBottomCenterTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveLeftTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveTopTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveRightTo, 'function');
    assert.equal(typeof Rect2D.prototype.moveBottomTo, 'function');
    assert.equal(typeof Rect2D.prototype.normalize, 'function');
    assert.equal(typeof Rect2D.prototype.setCoords, 'function');
    assert.equal(typeof Rect2D.prototype.getCoords, 'function');
    assert.equal(typeof Rect2D.prototype.onEdge, 'function');
    assert.equal(typeof Rect2D.prototype.contains, 'function');
    assert.equal(typeof Rect2D.prototype.containsCoord, 'function');
    assert.equal(typeof Rect2D.prototype.properlyContains, 'function');
    assert.equal(typeof Rect2D.prototype.properlyContainsCoord, 'function');
    assert.equal(typeof Rect2D.prototype.intersects, 'function');
    assert.equal(typeof Rect2D.prototype.intersectsCoord, 'function');
    assert.equal(typeof Rect2D.prototype.unite, 'function');
    assert.equal(typeof Rect2D.prototype.uniteCoord, 'function');
    assert.equal(typeof Rect2D.prototype.split, 'function');
    assert.equal(typeof Rect2D.prototype.splitX, 'function');
    assert.equal(typeof Rect2D.prototype.splitY, 'function');
    assert.equal(typeof Rect2D.prototype.united, 'function');
    assert.equal(typeof Rect2D.prototype.unitedCoord, 'function');
    assert.equal(typeof Rect2D.prototype.intersected, 'function');
    assert.equal(typeof Rect2D.prototype.intersectedCoord, 'function');

    var rect = new Rect2D();

    function testProperty(name, type, writable, classType) {
      type = type || 'number';
      if (typeof writable === 'undefined') {
        writable = true;
      }
      assert.equal(typeof rect[name], type);
      assert(rect.hasOwnProperty(name));
      assert.equal(Object.getOwnPropertyDescriptor(rect, name).writable, writable);
      if (classType) {
        assert(rect[name] instanceof classType);
      }
    }

    ['x', 'y', 'width', 'height', 'left', 'top', 'right', 'bottom'].forEach(name => {
        testProperty(name, 'number');
    })
    testProperty('diagonalLength', 'number', false);
    testProperty('size', 'object', true, addon.Size2D);


    ['topLeft', 'topRight', 'bottomLeft', 'bottomRight',
    'center',
    'topCenter', 'bottomCenter', 'leftCenter', 'rightCenter'].forEach(name => {
      testProperty(name, 'object', true, addon.Point2D);
    });

    done();
  });

  function testRect(r, left, top, right, bottom) {
    assert.equal(r.left, left);
    assert.equal(r.x, left);
    assert.equal(r.top, top);
    assert.equal(r.y, top);
    assert.equal(r.right, right);
    assert.equal(r.bottom, bottom);
    assert.equal(r.width, right - left);
    assert.equal(r.size.width, right - left);
    assert.equal(r.height, bottom - top);
    assert.equal(r.size.height, bottom - top);
    assert.equal(r.diagonalLength, Math.sqrt(r.width*r.width + r.height*r.height));

    assert.equal(r.topLeft.x, left);
    assert.equal(r.topLeft.y, top);
    assert.equal(r.topCenter.x, (right + left) / 2);
    assert.equal(r.topCenter.y, top);
    assert.equal(r.topRight.x, right);
    assert.equal(r.topRight.y, top);
    assert.equal(r.rightCenter.x, right);
    assert.equal(r.rightCenter.y, (top + bottom) / 2);
    assert.equal(r.bottomRight.x, right);
    assert.equal(r.bottomRight.y, bottom);
    assert.equal(r.bottomCenter.x, (right + left) / 2);
    assert.equal(r.bottomCenter.y, bottom);
    assert.equal(r.bottomLeft.x, left);
    assert.equal(r.bottomLeft.y, bottom);
    assert.equal(r.leftCenter.x, left);
    assert.equal(r.leftCenter.y, (top + bottom) / 2);

    assert.equal(r.center.x, (left + right) / 2);
    assert.equal(r.center.y, (top + bottom) / 2);

    assert.equal(r.horzRange.begin, left);
    assert.equal(r.horzRange.end, right);
    assert.equal(r.vertRange.begin, top);
    assert.equal(r.vertRange.end, bottom);
  }

  it('Rect2D constructor & property test', done => {
    function testCoord(left, top, right, bottom) {
      var rect;
      if (left === 0 && top === 0 && right === 0 && bottom === 0) {
        rect = new Rect2D();
        testRect(rect, left, top, right, bottom);
      }
      rect = new Rect2D(left, top, right - left, bottom - top);
      testRect(rect, left, top, right, bottom);
      rect = new Rect2D([left, top, right, bottom]);
      testRect(rect, left, top, right, bottom);
      rect = new Rect2D(new Point2D(left, top), new Size2D(right - left, bottom - top));
      testRect(rect, left, top, right, bottom);
    }

    testCoord(0, 0, 0, 0);
    testCoord(10, 10, 10, 10);
    testCoord(10, 20, 100, 300);
    testCoord(-10, -20, 0, 30);
    testCoord(-10, -20, -80, 50);
    testCoord(-10, -20, -80, -50);

    done();
  })
});
