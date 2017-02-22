// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('bindings')('realsense_person_tracking');
const assert = require('assert');
const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.PersonTracker, EventEmitter);

let options = {
  tracking: {
    enable: true,
  },
  skeleton: {
    enable: false,
  },
};

describe('Person Tracking Test Suite - Options', function() {
  let tracker = null;
  beforeEach(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

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

  it('Set=>Get options', function() {
    return new Promise(function(resolve, reject) {
    tracker.setPersonTrackerOptions(options).then(function() {
        return tracker.getPersonTrackerOptions();
      }).then(function(opt) {
        assert.equal(opt.tracking.enable, true);
        assert.equal(opt.skeleton.enable, false);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Create with options', function() {
    let newOptions = {
      tracking: {
        enable: false,
      },
      skeleton: {
        enable: true,
      },
    };
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(newOptions).then(function(inst) {
        tracker = inst;
        return tracker.getPersonTrackerOptions();
      }).then(function(opt) {
        assert.equal(opt.tracking.enable, false);
        assert.equal(opt.skeleton.enable, true);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
});
