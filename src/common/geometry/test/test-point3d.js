// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
var assert = require('assert');
var addon = require('bindings')('geometry');
var Point3D = addon.Point3D;

describe('Geometry Point3D', function() {
  it('Existence of Point3D', done => {
    assert.equal(typeof Point3D, 'function');
    assert.equal(typeof Point3D.distance, 'function');
    assert.equal(typeof Point3D.prototype, 'object');
    assert.equal(typeof Point3D.prototype.equal, 'function');
    assert.equal(typeof Point3D.prototype.setCoords, 'function');
    assert.equal(typeof Point3D.prototype.getCoords, 'function');

    var pt = new Point3D();

    assert.equal(typeof pt.x, 'number');
    assert.equal(typeof pt.y, 'number');
    assert.equal(typeof pt.z, 'number');

    assert(pt.hasOwnProperty('x'));
    assert(pt.hasOwnProperty('y'));
    assert(pt.hasOwnProperty('z'));

    assert.equal(Object.getOwnPropertyDescriptor(pt, 'x').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(pt, 'x').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(pt, 'y').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(pt, 'y').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(pt, 'z').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(pt, 'z').enumerable, true);

    done();
  });

  it('Point3D constructor test', done => {
    var pt = new Point3D();
    assert.equal(pt.x, 0);
    assert.equal(pt.y, 0);
    assert.equal(pt.z, 0);

    pt = new Point3D(123.5, 345.75, 2322);
    assert.equal(pt.x, 123.5);
    assert.equal(pt.y, 345.75);
    assert.equal(pt.z, 2322);

    pt = new Point3D([3333, 7322, 8888]);
    assert.equal(pt.x, 3333);
    assert.equal(pt.y, 7322);
    assert.equal(pt.z, 8888);

    done();
  });

  it('Point3D equal test', done => {
    var pt = new Point3D();
    assert(pt.equal(new Point3D()));
    assert(pt.equal(0, 0, 0));

    pt.x = 362391;
    pt.y = 123879;
    pt.z = 1048756;
    assert(pt.equal(new Point3D(362391, 123879, 1048756)));
    assert(pt.equal(362391, 123879, 1048756));


    // Floating point equal test
    pt.x = 0.3333333333333333;
    pt.y = 0;
    pt.z = 0;
    assert(pt.equal(1/3, 0, 0));

    pt.x = 0.33333333333333331;
    assert(pt.equal(1/3, 0, 0));

    pt.x = 0.3333333333333332;
    assert(!pt.equal(1/3, 0, 0));

    done();
  });

  it('Point3D coord test', done => {
    var pt = new Point3D(1342332, 4533352, 43762);

    assert(Array.isArray(pt.getCoords()));
    assert.equal(pt.getCoords().length, 3);
    assert.equal(pt.getCoords()[0], 1342332);
    assert.equal(pt.getCoords()[1], 4533352);
    assert.equal(pt.getCoords()[2], 43762);

    pt.setCoords([128, 386, 787]);
    assert.equal(pt.getCoords().length, 3);
    assert.equal(pt.getCoords()[0], 128);
    assert.equal(pt.getCoords()[1], 386);
    assert.equal(pt.getCoords()[2], 787);

    pt.setCoords(36278, 65563, 799);
    assert.equal(pt.getCoords().length, 3);
    assert.equal(pt.getCoords()[0], 36278);
    assert.equal(pt.getCoords()[1], 65563);
    assert.equal(pt.getCoords()[2], 799);

    done();
  });

  it('Point3D distance test', done => {

    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(2, 0, 0)), 2);
    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(-2, 0, 0)), 2);

    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(0, 3, 0)), 3);
    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(0, -3, 0)), 3);

    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(0, 0, 4)), 4);
    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(0, 0, 4)), 4);

    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(3, 4, 0)), 5);
    assert.equal(Point3D.distance(new Point3D(-1, -1, 0), new Point3D(2, 3, 0)), 5);
    assert.equal(Point3D.distance(new Point3D(-1, -2, 0), new Point3D(2, 2, 0)), 5);

    assert.equal(Point3D.distance(new Point3D(1, -1, 0), new Point3D(7, 4, 2)), Math.sqrt(36 + 25 + 4));

    assert.equal(Point3D.distance(new Point3D(0, 0, 0), new Point3D(1, 1, 1)), Math.sqrt(3));

    done();
  });
});
