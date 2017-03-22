// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const addon = require('bindings')('realsense_person_tracking');
let emitter = require('events').EventEmitter;
const assert = require('assert');
const path = require('path');
function inherits(target, source) {
  // eslint-disable-next-line
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(addon.PersonTracker, emitter);

describe('Person Tracking Test Suite - tracking', function() {
  let options = {
    tracking: {
      enable: true,
    },
  };
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

  function checkBlob(result) {
    for (let person of result.persons) {
      if(person.trackInfo.blobMask && person.trackInfo.blobMask.maskData) {
        return 1;
      }
    }
    return 0;
  }

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

  it('Start => getPersonInfo rejected', function() {
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options).then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        return tracker.start();
      }).then(function() {
        let info = tracker.getPersonInfo(0);
        assert(info instanceof Promise);
        return tracker.stop();
      }).then(function() {
        resolve();
      }).catch(function(e) {
        tracker.stop();
        reject(e);
      });
    });
  });

  it('Not crash when tracking is disabled', function() {
    let optionsNew = {
      tracking: {
        enable: false,
      },
    };
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(optionsNew).then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        return tracker.start();
      }).then(function() {
        setTimeout(function() {
          let state = tracker.state;
          assert.equal(state, 'running');
          resolve();
        }, 1000);
      }).catch(function(e) {
        reject('Exception happened');
      });
    });
  });

  it.skip('blob is defined', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(5000);
    let optionsNew = {
      tracking: {
        enable: true,
        enableBlob: true,
      },
    };
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(optionsNew).then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        tracker.on('persontracked', function(result) {
          if (checkBlob(result) === 1)
            resolve();
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('detectionMode coverage test', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(10000);
    let modes = ['auto', 'close-range', 'mid-range', 'far-range', 'full-range'];
    function singleModeTest(mode) {
      let optionsNew = {
        tracking: {
          detectionMode: mode,
        },
      };
      return addon.createPersonTracker(optionsNew, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        return tracker.start();
      }).then(function() {
        return tracker.reset();
      });
    };

    function getSerialPromises(modeArray) {
      return modeArray.reduce(function(promise, mode) {
        return promise.then(function() {
          return singleModeTest(mode);
        });
      }, Promise.resolve());
    };

    return new Promise(function(resolve, reject) {
      getSerialPromises(modes).then(function() {
        resolve();
      }).catch(function(e) {
        reject('Failed to cover all detection mode,' + e);
      });
    });
  });

  it('start/stop/reset tracking', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(5000);
    let optionsNew = {
      tracking: {
        enable: true,
      },
    };
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(optionsNew).then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions(cameraOptionsFromFile);
      }).then(function() {
        tracker.on('persontracked', function(result) {
          tracker.personTracking.startTrackingPerson(result.persons[0].trackInfo.id).then(
              function() {
            return tracker.personTracking.stopTrackingPerson(result.persons[0].trackInfo.id);
          }).then(function() {
            return tracker.personTracking.resetTracking();
          }).then(function() {
            resolve();
          }).catch(function() {
            reject();
          });
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
});
