// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
var assert = require('assert');
var addon = require('bindings')('geometry');
var Range = addon.Range;

describe('Geometry Range', function() {
  it('Existence of Range', done => {
    assert.equal(typeof Range, 'function');
    assert.equal(typeof Range.prototype, 'object');
    assert.equal(typeof Range.prototype.equal, 'function');
    assert.equal(typeof Range.prototype.isEmpty, 'function');
    assert.equal(typeof Range.prototype.isValid, 'function');

    var range = new Range();

    assert.equal(typeof range.begin, 'number');
    assert.equal(typeof range.end, 'number');
    assert.equal(typeof range.min, 'number');
    assert.equal(typeof range.max, 'number');

    assert(range.hasOwnProperty('begin'));
    assert(range.hasOwnProperty('end'));
    assert(range.hasOwnProperty('min'));
    assert(range.hasOwnProperty('max'));

    assert.equal(Object.getOwnPropertyDescriptor(range, 'begin').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(range, 'begin').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(range, 'end').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(range, 'end').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(range, 'min').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(range, 'min').enumerable, true);

    assert.equal(Object.getOwnPropertyDescriptor(range, 'max').writable, true);
    assert.equal(Object.getOwnPropertyDescriptor(range, 'max').enumerable, true);

    done();
  });

  it('Range constructor test', done => {
    var r = new Range();
    assert.equal(r.begin, 0);
    assert.equal(r.end, 0);
    assert.equal(r.min, 0);
    assert.equal(r.max, 0);

    r = new Range(120, 448);
    assert.equal(r.begin, 120);
    assert.equal(r.end, 448);
    assert.equal(r.min, 120);
    assert.equal(r.max, 448);

    done();
  });

  it('Range attribute alias test', done => {
    var r = new Range(77, 66);

    r.min = 999;
    assert.equal(r.begin, 999);
    r.max = 666;
    assert.equal(r.end, 666);

    r.begin = 222;
    assert.equal(r.min, 222);
    r.end = 555;
    assert.equal(r.max, 555);

    done();
  });

  it('Range (properly) contains test', done => {
    var r = new Range(1, 10);

    assert(!r.contains(-1));
    assert(!r.properlyContains(-1));
    assert(!r.contains(0));
    assert(!r.properlyContains(0));
    assert(r.contains(1));
    assert(!r.properlyContains(1));
    assert(r.contains(2));
    assert(r.contains(3));
    assert(r.contains(8));
    assert(r.contains(9));
    assert(r.contains(10));
    assert(!r.properlyContains(10));
    assert(!r.contains(11));
    assert(!r.properlyContains(11));
    assert(!r.contains(12));
    assert(!r.properlyContains(12));

    done();
  });

  it('Range equal test', done => {
    var r = new Range();
    assert(r.equal(new Range()));
    assert(r.equal(0, 0));

    r.min = 362391;
    r.max = 123879;
    assert(r.equal(new Range(362391, 123879)));
    assert(r.equal(362391, 123879));

    // Floating point equal test
    r.min = 0.3333333333333333;
    r.max = 0;
    assert(r.equal(1/3, 0));

    r.min = 0.33333333333333331;
    assert(r.equal(1/3, 0));

    r.min = 0.3333333333333332;
    assert(!r.equal(1/3, 0));

    done();
  });

  it('Range empty test', done => {
    var r = new Range();
    assert(r.isEmpty());

    r.max = 1;
    assert(! r.isEmpty());

    r.begin = 1;
    assert(r.isEmpty());

    r.begin = 21;
    assert(!r.isEmpty());

    done();
  });

  it('Range valid test', done => {
    var r = new Range();
    assert(r.isValid());

    r.max = 1;
    assert(r.isValid());

    r.begin = 1;
    assert(r.isValid());

    r.begin = 21;
    assert(!r.isValid());

    done();
  });

});
