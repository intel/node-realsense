// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_person_tracking');
const path = require('path');
const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.PersonTracker, EventEmitter);

describe('Person Tracking Test Suite - State - Full', function() {
  let tracker = undefined;
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

  beforeEach(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
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

  it('create - start - reset', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.reset();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
      }).then(function() {
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('create - start - reset - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.reset();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        return tracker.stop();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.resume();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'detecting') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - pause - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
        return tracker.stop();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('create - start - pause - reset - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
        return tracker.reset();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        return tracker.stop();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.pause().then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.resume().then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      return tracker.stop().then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - reset', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      return tracker.reset().then(function() {
        assert.equal(tracker.state, 'ready');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('create - start - start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.start();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'detecting') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - pause - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
        return tracker.resume();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('create - start - stop- start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.stop();
      }).then(function() {
        assert.equal(tracker.state, 'ready');
        return tracker.start();
      }).then(function() {
        assert.equal(tracker.state, 'detecting');
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('create - start - pause - start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
        return tracker.start();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - pause - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60 * 1000);
    return new Promise(function(resolve, reject) {
      assert.equal(tracker.state, 'ready');
      tracker.start().then(function() {
        assert.equal(tracker.state, 'detecting');
        return tracker.pause();
      }).then(function() {
        assert.equal(tracker.state, 'paused');
        return tracker.pause();
      }).then(function() {
        reject('should fail but promis got resolved');
      }).catch(function(e) {
        if (tracker.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });
});
