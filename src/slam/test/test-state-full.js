// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_slam');

const EventEmitter = require('events').EventEmitter;
function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.Instance, EventEmitter);

let trackingEvent = 'tracking';
let errorEvent = 'error';

function addListeners(instance) {
  instance.on('newListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = true;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = true;
  });
  instance.on('removeListener', function(event) {
    if (event === trackingEvent && instance.listenerCount(event) === 0)
      instance.enableTrackingEvent = false;
    if (event === errorEvent && instance.listenerCount(event) === 0)
      instance.enableErrorEvent = false;
  });
}

describe('SLAM Test Suite - State - full', function() {
  let instance = undefined;

  beforeEach(function() {
    return new Promise((resolve, reject) => {
      instance = undefined;
      addon.createInstance().then((inst) => {
        instance = inst;
        addListeners(instance);
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
      if (instance && instance.state === 'tracking') {
        instance.stop().then(() => {
          resolve();
        }).catch((e) => {
          reject(e);
        });
      } else if (instance && instance.state === 'paused') {
        instance.resume().then(() => {
          return instance.stop();
        }).then(() => {
          resolve();
        }).catch((e) => {
          reject(e);
        });
      } else if (instance && instance.state === 'ready') {
        resolve();
      } else {
        reject('instance is undefined!');
      }
    });
  });

  it('create - start - reset', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.reset();
      }).then(() => {
        assert.equal(instance.state, 'ready');
      }).then(() => {
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('create - start - reset - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.reset();
      }).then(() => {
        assert.equal(instance.state, 'ready');
        return instance.stop();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });
  it.skip('create - start - pause - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.pause();
      }).then(() => {
        assert.equal(instance.state, 'paused');
        return instance.stop();
      }).then(() => {
        assert.equal(instance.state, 'ready');
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
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.pause();
      }).then(() => {
        assert.equal(instance.state, 'paused');
        return instance.reset();
      }).then(() => {
        assert.equal(instance.state, 'ready');
        return instance.stop();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it.skip('create - start - pause - -restart - stop', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.pause();
      }).then(() => {
        assert.equal(instance.state, 'paused');
        return instance.restart();
      }).then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.stop();
      }).then(() => {
        assert.equal(instance.state, 'ready');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('create - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.pause().then(() => {
      }).catch((e) => {
        reject('should fail but promis got resolved');
        if (instance.state === 'ready') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it.skip('create - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.resume().then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'ready') {
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
      assert.equal(instance.state, 'ready');
      return instance.stop().then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'ready') {
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
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.start();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'tracking') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it.skip('create - start - resume', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.resume();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'tracking') {
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
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.stop();
      }).then(() => {
        assert.equal(instance.state, 'ready');
        return instance.start();
      }).then(() => {
        assert.equal(instance.state, 'tracking');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('create - start - pause - start', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.pause();
      }).then(() => {
        assert.equal(instance.state, 'paused');
        return instance.start();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });

  it.skip('create - start - pause - pause', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50 * 1000);
    return new Promise((resolve, reject) => {
      assert.equal(instance.state, 'ready');
      instance.start().then(() => {
        assert.equal(instance.state, 'tracking');
        return instance.pause();
      }).then(() => {
        assert.equal(instance.state, 'paused');
        return instance.pause();
      }).then(() => {
        reject('should fail but promis got resolved');
      }).catch((e) => {
        if (instance.state === 'paused') {
          resolve();
        } else {
          reject(e);
        }
      });
    });
  });
});
