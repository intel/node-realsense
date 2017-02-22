// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

/* global afterEach, describe, it */
/* eslint no-invalid-this: "off" */
'use strict';
const emitter = require('events').EventEmitter;
const assert = require('assert');
const pt = require('bindings')('realsense_person_tracking');

function inherits(target, source) {
  // eslint-disable-next-line
  for (var k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}

inherits(pt.PersonTracker, emitter);
function testOption(opt, par, value, data) {
  describe('check trackerOptions ' + opt, function() {
    let personTracker;
    afterEach(function() {
      return personTracker.reset();
    });

    it('[createPersonTracker] Checking ' + opt + ' - ' + par + ': ' + value, function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        pt.createPersonTracker(data).then((instance) => {
          personTracker = instance;
          return personTracker.getPersonTrackerOptions();
        }).then((opts) => {
          assert.equal(opts[opt][par], value);
          return personTracker.start();
        }).then(() => {
            resolve();
        }).catch(function(e) {
            reject(e);
        });
      });
    });

    it('[setPersonTrackerOptions] Checking ' + opt + ' - ' + par + ': ' + value, function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        pt.createPersonTracker().then((instance) => {
          personTracker = instance;
          return personTracker.setPersonTrackerOptions(data);
        }).then(() => {
          return personTracker.getPersonTrackerOptions();
        }).then((opts) => {
          assert.equal(opts[opt][par], value);
          return personTracker.start();
        }).then(() => {
            resolve();
        }).catch(function(e) {
            reject(e);
        });
      });
    });
  });
}

function negativeTestOption(opt, par, value, data) {
  describe('check trackerOptions ' + opt, function() {
    let personTracker;

    it('[createPersonTracker] Checking negative' + opt + ' - ' + par + ': ' + value, function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        pt.createPersonTracker(data).then((instance) => {
          reject('Should reject but promise got resolved');
        }).catch(function(e) {
          resolve();
        });
      });
    });

    it('[setPersonTrackerOptions] Checking negative' + opt + ' - ' + par + ':' + value, function() {
      this.timeout(60000);
      return new Promise((resolve, reject) => {
        pt.createPersonTracker().then((instance) => {
          personTracker = instance;
          return personTracker.setPersonTrackerOptions(data);
        }).then(() => {
          reject('Should reject but promise got resolved');
        }).catch(function(e) {
          resolve();
        });
      });
    });
  });
}

let sets = {
  // skip expression for MW Beta3 RC5
  // expression: {
  //   disableAllExpressions: [true, false],
  //   enableAllExpressions: [true, false],
  //   enableAnger: [true, false],
  //   enableContempt: [true, false],
  //   enableDisgust: [true, false],
  //   enable: [true, false],
  //   enableFear: [true, false],
  //   enableHappy: [true, false],
  //   enableNeutral: [true, false],
  //   enableSad: [true, false],
  //   enableSurpise: [true, false],
  //   maxTrackedPerson: [1, 2],
  // },
  gesture: {
    disableAllGestures: [true, false],
    enableAllGestures: [true, false],
    enable: [true, false],
    enablePointing: [true, false],
    maxTrackedPerson: [1, 2],
  },
  lying: {
    enable: [true, false],
    maxTrackedPerson: [1, 2],
  },
  personFace: {
    enableHeadPose: [true, false],
    enableFaceLandmarks: [true, false],
  },
  pose: {
    enable: [true, false],
    maxTrackedPerson: [1, 2],
  },
  recognition: {
    enable: [true, false],
    policy: ['standard'],
    useMultiFrame: [true, false],
  },
  skeleton: {
    enable: [true, false],
    maxTrackedPerson: [1, 2],
    trackingArea: ['full-body', 'full-body-rough', 'upper-body', 'upper-body-rough'],
  },
  tracking: {
    enable: [true],
    enableBlob: [true, false],
    enableDetectionFromFar: [true, false],
    enableHeadBoundingBox: [true, false],
    enableSegmentation: [true, false],
    maxTrackedPerson: [1, 2],
    trackingMode: ['following', 'interactive', 'single-person'],
    detectionMode: ['auto', 'close-range', 'mid-range', 'far-range', 'full-range'],
  },
};
let negativeSets = {
  gesture: {
    disableAllGestures: [123, 'true'],
    enableAllGestures: [123, 'dummy'],
    enable: [123, 'dummy'],
    enablePointing: [123, 'dummy'],
    maxTrackedPerson: ['123'],
  },
  lying: {
    enable: [123, 'true'],
    maxTrackedPerson: ['123'],
  },
  personFace: {
    enableHeadPose: [123, 'true'],
    enableFaceLandmarks: [123, 'true'],
  },
  pose: {
    enable: [123, 'true'],
    maxTrackedPerson: ['123'],
  },
  recognition: {
    enable: [123, 'true'],
    policy: ['standards'],
    useMultiFrame: [123, 'true'],
  },
  skeleton: {
    enable: [123, 'true'],
    maxTrackedPerson: ['123'],
    trackingArea: ['full-bodys'],
  },
  tracking: {
    enable: [123, 'true'],
    enableBlob: [123, 'true'],
    enableDetectionFromFar: [123, 'true'],
    enableHeadBoundingBox: [123, 'true'],
    enableSegmentation: [123, 'true'],
    maxTrackedPerson: ['123'],
    trackingMode: ['followings'],
    detectionMode: ['autos'],
  },
};
// eslint-disable-next-line guard-for-in
for(let i in sets) {
  // eslint-disable-next-line guard-for-in
  for(let j in sets[i]) {
    // eslint-disable-next-line guard-for-in
    for(let k in sets[i][j]) {
      let data = {};
      data[i] = {};
      data[i][j] = {};
      data[i][j] = sets[i][j][k];
      testOption(i, j, sets[i][j][k], data);
    }
  }
}
// eslint-disable-next-line guard-for-in
for(let i in negativeSets) {
  // eslint-disable-next-line guard-for-in
  for(let j in negativeSets[i]) {
    // eslint-disable-next-line guard-for-in
    for(let k in negativeSets[i][j]) {
      let data = {};
      data[i] = {};
      data[i][j] = {};
      data[i][j] = negativeSets[i][j][k];
      negativeTestOption(i, j, negativeSets[i][j][k], data);
    }
  }
}
