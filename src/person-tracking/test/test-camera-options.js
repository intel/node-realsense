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

let cameraOptions = {
  color: {
    width: 320,
    height: 240,
    frameRate: 30,
    isEnabled: true,
  },
  depth: {
    width: 628,
    height: 468,
    frameRate: 30,
    isEnabled: true,
  },
};

describe('Person Tracking Test Suite - Camera Options', function() {
  let tracker = null;
  afterEach(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout( 5 * 1000);
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

  it('Set camera options', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptions);
      }).then(function() {
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Create with camera options', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptions).then(function(inst) {
        tracker = inst;
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(options.color.width, 320);
        assert.equal(options.color.height, 240);
        assert.equal(options.color.frameRate, 30);
        assert.equal(options.color.isEnabled, true);
        assert.equal(options.depth.width, 628);
        assert.equal(options.depth.height, 468);
        assert.equal(options.depth.frameRate, 30);
        assert.equal(options.color.isEnabled, true);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Get default camera options', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(typeof options, 'object');
        assert.equal(typeof options.color, 'object');
        assert.equal(typeof options.color.width, 'number');
        assert.equal(typeof options.color.height, 'number');
        assert.equal(typeof options.color.isEnabled, 'boolean');
        assert.equal(typeof options.color.frameRate, 'number');

        assert.equal(typeof options.depth, 'object');
        assert.equal(typeof options.depth.width, 'number');
        assert.equal(typeof options.depth.height, 'number');
        assert.equal(typeof options.depth.isEnabled, 'boolean');
        assert.equal(typeof options.depth.frameRate, 'number');

        assert.equal(options.color.width, 640);
        assert.equal(options.color.height, 480);
        assert.equal(options.color.frameRate, 30);
        assert.equal(options.depth.width, 320);
        assert.equal(options.depth.height, 240);
        assert.equal(options.depth.frameRate, 30);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Set and get camera options', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        let newOptions = {
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
        };
        return tracker.setCameraOptions(newOptions);
      }).then(function() {
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(options.color.width, 320);
        assert.equal(options.color.height, 240);
        assert.equal(options.color.frameRate, 30);
        assert.equal(options.color.isEnabled, true);
        assert.equal(options.depth.width, 320);
        assert.equal(options.depth.height, 240);
        assert.equal(options.depth.frameRate, 30);
        assert.equal(options.color.isEnabled, true);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Invalid camera options is rejected', function() {
    return new Promise(function(resolve, reject) {
      let invalidOptions = {
        color: {
          width: 32,
          height: 24,
          frameRate: 100,
          isEnabled: true,
        },
        depth: {
          width: 320,
          height: 240,
          frameRate: 30,
          isEnabled: true,
        },
      };
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(invalidOptions);
      }).then(function() {
        reject('Should not accept wrong camera options');
      }).catch(function(e) {
        resolve(e);
      });
    });
  });
});
