// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
var assert = require('assert');
var addon = require('bindings')('geometry');
var Point2D = addon.Point2D;

describe('Geometry Point2D', function() {
  it('Existence of Point2D', done => {
    assert.equal(typeof Point2D, 'function');
    assert.equal(typeof Point2D.distance, 'function');
    assert.equal(typeof Point2D.prototype, 'object');
    assert.equal(typeof Point2D.prototype.equal, 'function');
    assert.equal(typeof Point2D.prototype.setCoords, 'function');
    assert.equal(typeof Point2D.prototype.getCoords, 'function');

    var pt = new Point2D();

    assert.equal(typeof pt.x, 'number');
    assert.equal(typeof pt.y, 'number');

    assert(pt.hasOwnProperty('x'));
    assert(pt.hasOwnProperty('y'));

    assert.equal(Object.getOwnPropertyDescriptor(pt, 'x').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(pt, 'x').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(pt, 'y').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(pt, 'y').enumerable, true);

    done();
  });

  it('Point2D constructor test', done => {
    var pt = new Point2D();
    assert.equal(pt.x, 0);
    assert.equal(pt.y, 0);

    pt = new Point2D(123.5, 345.75);
    assert.equal(pt.x, 123.5);
    assert.equal(pt.y, 345.75);

    pt = new Point2D([3333, 7322]);
    assert.equal(pt.x, 3333);
    assert.equal(pt.y, 7322);

    done();
  });

  it('Point2D equal test', done => {
    var pt = new Point2D();
    assert(pt.equal(new Point2D()));
    assert(pt.equal(0, 0));

    pt.x = 362391;
    pt.y = 123879;
    assert(pt.equal(new Point2D(362391, 123879)));
    assert(pt.equal(362391, 123879));

    // Floating point equal test
    pt.x = 0.3333333333333333;
    pt.y = 0;
    assert(pt.equal(1/3, 0));

    pt.x = 0.33333333333333331;
    assert(pt.equal(1/3, 0));

    pt.x = 0.3333333333333332;
    assert(!pt.equal(1/3, 0));

    done();
  });

  it('Point2D coord test', done => {
    var pt = new Point2D(1342332, 4533352);

    assert(Array.isArray(pt.getCoords()));
    assert.equal(pt.getCoords().length, 2);
    assert.equal(pt.getCoords()[0], 1342332);
    assert.equal(pt.getCoords()[1], 4533352);

    pt.setCoords([128, 386]);
    assert.equal(pt.getCoords().length, 2);
    assert.equal(pt.getCoords()[0], 128);
    assert.equal(pt.getCoords()[1], 386);

    pt.setCoords(36278, 65563);
    assert.equal(pt.getCoords().length, 2);
    assert.equal(pt.getCoords()[0], 36278);
    assert.equal(pt.getCoords()[1], 65563);

    done();
  });

  it('Point2D distance test', done => {

    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(2, 0)), 2);
    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(-2, 0)), 2);

    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(0, 3)), 3);
    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(0, -3)), 3);

    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(3, 4)), 5);
    assert.equal(Point2D.distance(new Point2D(-1, -1), new Point2D(2, 3)), 5);
    assert.equal(Point2D.distance(new Point2D(-1, -2), new Point2D(2, 2)), 5);

    assert.equal(Point2D.distance(new Point2D(0, 0), new Point2D(1, 1)), Math.sqrt(2));

    done();
  });
});
