// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const addon = require('bindings')('realsense_person_tracking');
const assert = require('assert');
const path = require('path');
const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.PersonTracker, EventEmitter);

describe('Person Tracking Test Suite - States', function() {
  let tracker = null;
  const cameraOptionsFromFile = {
    color: {
      width: 320,
      height: 240,
      frameRate: 30,
      isEnabled: true,
    },
    depth: {
      width: 320,
      height: 240,
      frameRate: 30,
      isEnabled: true,
    },
    playbackPathForTesting: path.resolve(path.join(
        '..', 'common', 'tests', 'data', 'pt',
        'pt_zr300_sync_color-320x240-rgb8-30_depth-320x240-z16-30_frames-200.out')),
  };

  afterEach(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      if (tracker) {
        tracker.reset().then(function() {
          resolve();
        }).catch(function(e) {
          reject(e);
        });
      }
    });
  });

  it('Start => Stop', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        assert.equal(tracker.state, 'ready');
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        return tracker.start();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.stop();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('start => pause => resume => stop', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        assert.equal(tracker.state, 'ready');
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        return tracker.start();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
      }).then(function() {
        return tracker.resume();
      }).then(function() {
        return new Promise(function(innerresolve, innerreject) {
          setTimeout(function() {
            assert.equal(tracker.state, 'detecting');
            innerresolve();
          }, 300);
        });
      }).then(function() {
        return tracker.stop();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('start => reset => start => stop', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        assert.equal(tracker.state, 'ready');
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        return tracker.start();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.reset();
      }).then(function() {
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        return tracker.start();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.stop();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
});
