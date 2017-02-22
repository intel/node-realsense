// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
// const assert = require('assert');
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

let or = undefined;

const cameraOptionsFromFile = {
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

describe('Object Recognition Test Suite - Run', function() {
  function testRun(mode) {
    return new Promise((resolve, reject) => {
      let eventCounter = 0;

      function processEvent() {
        if (or && eventCounter++ > 2) {
          function tryStop() {
            or.stop().then(function() {
              resolve();
            });
            or.removeAllListeners('recognition');
            or.removeAllListeners('localization');
            or.removeAllListeners('tracking');
            or = undefined;
          }
          tryStop();
        }
      }

      const options = {
        mode: mode,
        confidenceThreshold: 0.15,
        computeEngine: 'CPU',
        enableSegmentation: true,
        maxReturnObjectCount: 3,
        enableObjectCenterEstimation: false,
      };

      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setCameraOptions(cameraOptionsFromFile);
      }).then(() => {
        return or.setObjectRecognitionOptions(options);
      }).then(() => {
        or.on('localization', function(e) {
          if (mode == 'localization') {
            processEvent();
          } else {
            reject('Unexpected localization event fired with mode == single-recognition!');
          }
        });

        or.on('tracking', function(e) {
          if (mode == 'tracking') {
            processEvent();
          } else {
            reject('Unexpected tracking event fired with mode == single-recognition!');
          }
        });

        or.on('recognition', function(e) {
          if (mode == 'single-recognition') {
            processEvent();
          } else {
            reject('Unexpected recognition event fired with mode == single-recognition!');
          }
        });

        return or.start();
      }).then(() => {
        // console.log('or.started');
      }).catch((e) => {
        reject(e);
      });
    });
  }

  it('Run or on single-recognition mode and verify events can be fired', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return testRun('single-recognition');
  });

  it('Run or on localization mode and verify events can be fired', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return testRun('localization');
  });

  it('Run OR with options set by createObjectRecognizer(), mode: single-recognition', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      let eventCounter = 0;

      function processEvent() {
        if (or && eventCounter++ > 2) {
          or.stop().then(function() {
            resolve();
          });
          or.removeAllListeners('recognition');
          or.removeAllListeners('localization');
          or.removeAllListeners('tracking');
          or = undefined;
        }
      }

      const orOptions = {
        mode: 'single-recognition',
        confidenceThreshold: 0.15,
        computeEngine: 'CPU',
        enableSegmentation: true,
        maxReturnObjectCount: 3,
        enableObjectCenterEstimation: false,
      };

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
        playbackPathForTesting: cameraOptionsFromFile.playbackPathForTesting,
      };

      addon.createObjectRecognizer(orOptions, cameraOptions).then((objectRecognizer) => {
        or = objectRecognizer;

        or.on('localization', function(e) {
          reject('Unexpected recognition event fired with mode == single-recognition!');
        });

        or.on('tracking', function(e) {
          reject('Unexpected tracking event fired with mode == single-recognition!');
        });

        or.on('recognition', function(e) {
          processEvent();
        });

        return or.start();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Run OR with options set by createObjectRecognizer(), mode: localization', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      let eventCounter = 0;

      function processEvent() {
        if (or && eventCounter++ > 2) {
          or.stop().then(function() {
            resolve();
          });
          or.removeAllListeners('recognition');
          or.removeAllListeners('localization');
          or.removeAllListeners('tracking');
          or = undefined;
        }
      }

      const options = {
        mode: 'localization',
        confidenceThreshold: 0.15,
        computeEngine: 'CPU',
        enableSegmentation: true,
        maxReturnObjectCount: 3,
        enableObjectCenterEstimation: false,
      };

      addon.createObjectRecognizer(options, cameraOptionsFromFile).then((objectRecognizer) => {
        or = objectRecognizer;

        or.on('localization', function(e) {
          processEvent();
        });

        or.on('tracking', function(e) {
          reject('Unexpected tracking event fired with mode == single-recognition!');
        });

        or.on('recognition', function(e) {
          reject('Unexpected recognition event fired with mode == single-recognition!');
        });

        return or.start();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it.skip('Run OR by skipping every 60 frames', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    let startTime = undefined;
    return new Promise((resolve, reject) => {
      let eventCounter = 0;

      function processEvent() {
        if (!startTime) {
          startTime = new Date().getTime();
        }

        if (or && eventCounter++ > 2) {
          let diff = new Date().getTime() - startTime;
          // 30 FPS, 60 frames = 2.0 seconds
          // 2.0 sec * 3 results = 6.0 seconds
          if (diff < 6 * 1000) {
            reject('Time consumption is less than 6.0 seconds');
          }
          or.stop().then(function() {
            resolve();
          });
          or.removeAllListeners('recognition');
          or.removeAllListeners('localization');
          or.removeAllListeners('tracking');
          or = undefined;
        }
      }

      const options = {
        mode: 'single-recognition',
        framesSkipping: 60,
        confidenceThreshold: 0.15,
        computeEngine: 'CPU',
        enableSegmentation: true,
        maxReturnObjectCount: 3,
        enableObjectCenterEstimation: false,
      };

      addon.createObjectRecognizer(options).then((objectRecognizer) => {
        or = objectRecognizer;

        or.on('localization', function(e) {
          reject('Unexpected localization event fired with mode == single-recognition!');
        });

        or.on('tracking', function(e) {
          reject('Unexpected tracking event fired with mode == single-recognition!');
        });

        or.on('recognition', function(e) {
          processEvent();
        });

        return or.start();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Ensure run()/stop() can be invoked without respecting promise usage', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(50000);
    return new Promise((resolve, reject) => {
      let or = undefined;
      let stopRejectionCaught = false;
      addon.createObjectRecognizer().then((i) => {
        or = i;
        return i;
      }).then((i) => {
        let promise = or.start();
        or.stop().then(() => {
          reject('At this moment stop() can\'t be resolved!');
        }).catch(() => {
          stopRejectionCaught = true;
        });
        return promise;
      }).then(() => {
        return or.stop();
      }).then(() => {
        if (stopRejectionCaught) {
          resolve();
        } else {
          reject('Some conditions are not as expected');
        }
      }).catch((e) => {
        reject('Exception caught' + e);
      });
    });
  });
});
