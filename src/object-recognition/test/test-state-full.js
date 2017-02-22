// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');
const path = require('path');

const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.ObjectRecognizer, EventEmitter);

describe('Object Recognition Test Suite - State - misc', function() {
  let or = undefined;

  beforeEach(function() {
    const cameraOptions = {
      color: {
        width: 640,
        height: 480,
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
          '..', 'common', 'tests', 'data', 'or',
          'or_zr300_sync_color-640x480-rgb8-30_depth-320x240-z16-30_frames-200.out')),
    };
    return new Promise((resolve, reject) => {
      or = undefined;
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setCameraOptions(cameraOptions);
      }).then(() => {
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  afterEach(function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      if (or && or.state === 'running') {
        or.stop().then(() => {
          resolve();
        }).catch((e) => {
          reject(e);
        });
      } else if (or && or.state === 'paused') {
        or.resume().then(() => {
          return or.stop();
        }).then(() => {
          resolve();
        }).catch((e) => {
          reject(e);
        });
      } else if (or && or.state === 'ready') {
        resolve();
      } else {
        reject('or is undefined!');
      }
    });
  });

  it.skip('create - start - reset - restart', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.reset();
      }).then(() => {
        assert.equal(or.state, 'ready');
        return or.restart();
      }).then(() => {
        assert.equal(or.state, 'running');
      }).then(() => {
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('create - start - restart - pause - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.restart();
      }).then(() => {
        assert.equal(or.state, 'running');
        return or.pause();
      }).then(() => {
        assert.equal(or.state, 'paused');
        return or.stop();
      }).then(() => {
        assert.equal(or.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('create - start - pause - reset - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.pause();
      }).then(() => {
        assert.equal(or.state, 'paused');
        return or.reset();
      }).then(() => {
        assert.equal(or.state, 'ready');
        return or.stop();
      }).then(() => {
        assert.equal(or.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('create - start - pause - restart - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.pause();
      }).then(() => {
        assert.equal(or.state, 'paused');
        return or.restart();
      }).then(() => {
        assert.equal(or.state, 'running');
        return or.stop();
      }).then(() => {
        assert.equal(or.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('create - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.pause().then(() => {
        reject('pause got success callback');
      }).catch((e) => {
        if (or.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.resume().then(() => {
        reject('resume got success callback');
      }).catch((e) => {
        if (or.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      return or.stop().then(() => {
        reject('stop got success callback');
      }).catch((e) => {
        if (or.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.start();
      }).then(() => {
         reject('start got success callback');
      }).catch((e) => {
        if (or.state === 'running') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.resume();
      }).then(() => {
         reject('resume got success callback');
      }).catch((e) => {
        if (or.state === 'running') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - stop- start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.stop();
      }).then(() => {
        assert.equal(or.state, 'ready');
        return or.start();
      }).then(() => {
        assert.equal(or.state, 'running');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('create - start - pause - start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.pause();
      }).then(() => {
        assert.equal(or.state, 'paused');
        return or.start();
      }).then(() => {
         reject('start got success callback');
      }).catch((e) => {
        if (or.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it('create - start - pause - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(or.state, 'ready');
      or.start().then(() => {
        assert.equal(or.state, 'running');
        return or.pause();
      }).then(() => {
        assert.equal(or.state, 'paused');
        return or.pause();
      }).then(() => {
         reject('pause got success callback');
      }).catch((e) => {
        if (or.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });
});
