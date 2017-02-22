// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
var assert = require('assert');
var addon = require('bindings')('geometry');
var Size2D = addon.Size2D;

describe('Geometry Size2D', function() {
  it('Existence of Size2D', done => {
    assert.equal(typeof Size2D, 'function');
    assert.equal(typeof Size2D.prototype, 'object');
    assert.equal(typeof Size2D.prototype.equal, 'function');
    assert.equal(typeof Size2D.prototype.isEmpty, 'function');
    assert.equal(typeof Size2D.prototype.isValid, 'function');

    var sz = new Size2D();

    assert.equal(typeof sz.width, 'number');
    assert.equal(typeof sz.height, 'number');
    assert.equal(typeof sz.cx, 'number');
    assert.equal(typeof sz.cy, 'number');

    assert(sz.hasOwnProperty('width'));
    assert(sz.hasOwnProperty('height'));
    assert(sz.hasOwnProperty('cx'));
    assert(sz.hasOwnProperty('cy'));

    assert.equal(Object.getOwnPropertyDescriptor(sz, 'width').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(sz, 'width').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(sz, 'height').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(sz, 'height').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(sz, 'cx').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(sz, 'cx').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(sz, 'cy').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(sz, 'cy').enumerable, true);

    done();
  });

  it('Size2D constructor test', done => {
    var s = new Size2D();
    assert.equal(s.width, 0);
    assert.equal(s.height, 0);
    assert.equal(s.cx, 0);
    assert.equal(s.cy, 0);

    s = new Size2D(120, 448);
    assert.equal(s.width, 120);
    assert.equal(s.height, 448);
    assert.equal(s.cx, 120);
    assert.equal(s.cy, 448);

    done();
  });

  it('Size2D attribute alias test', done => {
    var s = new Size2D(77, 66);

    s.width = 999;
    assert.equal(s.cx, 999);
    s.height = 666;
    assert.equal(s.cy, 666);

    s.cx = 345;
    assert.equal(s.width, 345);
    s.cy = 889;
    assert.equal(s.height, 889);

    done();
  });

  it('Size2D equal test', done => {
    var s = new Size2D();
    assert(s.equal(new Size2D()));
    assert(s.equal(0, 0));

    s.cx = 362391;
    s.cy = 123879;
    assert(s.equal(new Size2D(362391, 123879)));
    assert(s.equal(362391, 123879));

    // Floating point equal test
    s.cx = 0.3333333333333333;
    s.cy = 0;
    assert(s.equal(1/3, 0));

    s.cx = 0.33333333333333331;
    assert(s.equal(1/3, 0));

    s.cx = 0.3333333333333332;
    assert(!s.equal(1/3, 0));

    done();
  });

  it('Size2D empty test', done => {
    var s = new Size2D();
    assert(s.isEmpty());

    s.cy = 1;
    assert(s.isEmpty());

    s.cx = 1;
    assert(! s.isEmpty());

    s.cy = 0;
    assert(s.isEmpty());

    s.cy = 21;
    assert(!s.isEmpty());

    done();
  });

  it('Size2D valid test', done => {
    var s = new Size2D();
    assert(s.isValid());

    s.cx = 1;
    assert(s.isValid());

    s.cy = 1;
    assert(s.isValid());

    s.cx = 0;
    assert(s.isValid());

    s.cx = -1;
    assert(! s.isValid());

    s.cx = 0;
    s.cy = -1;
    assert(! s.isValid());

    s.cx = 0;
    s.cy = -2;
    assert(! s.isValid());


    done();
  });

});
