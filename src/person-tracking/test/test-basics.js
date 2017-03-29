// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const spawnNpmInstall = require('spawn-npm-install');
const testNpmInstall = require('../common/scripts/test-npm-install.js');
const tmp = require('tmp');
const addon = require('bindings')('realsense_person_tracking');
const assert = require('assert');

describe('Person Tracking Test Suite - Basics', function() {
  it('Make the tarball', function() {
    return testNpmInstall.makeTarball();
  });

  it('Make sure can be installed via npm install <tarball>', function() {
    // eslint-disable-next-line
    this.timeout(600*1000);

    return testNpmInstall.installFromTarball(tmp, spawnNpmInstall);
  });

  it('Make sure methods are all there', function(done) {
    let g = {
      PersonRecognition: ['getAllRecognitionIDs', 'clearDatabase', 'exportDatabase',
                        'importDatabase', 'recognizeAllPersons', 'registerPerson',
                        'unRegisterPerson', 'recognitionIDExist', 'reinforceRegistration',
                        'recognizePerson', 'querySimilarityScoreFromPerson',
                        'removePersonDescriptor', 'getPersonDescriptorIDs'],
      PersonTracking: ['startTrackingPerson', 'stopTrackingPerson', 'resetTracking'],
      PersonTracker: ['start', 'stop', 'getPersonTrackerOptions',
                      'setPersonTrackerOptions', 'pause', 'resume', 'reset',
                      'getCameraOptions', 'setCameraOptions', 'getPersonInfo'],
    };
    // eslint-disable-next-line
    for(let k in g) {
      let Instance = addon[k];
      g[k].forEach((methodName) => {
        assert.equal(typeof Instance.prototype[methodName], 'function');
      });
    }
    done();
  });

  it('Make sure PersonTracker properties are all there', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(30000);
    return new Promise((resolve, reject) => {
      addon.createPersonTracker().then(function(instance) {
        let g = {
          // attributeName: [type, writable]
          personRecognition: ['object', false],
          personTracking: ['object', false],
          state: ['string', false],
        };
        // eslint-disable-next-line
        for(let k in g) {
          assert(instance.hasOwnProperty(k));
          assert.equal(typeof instance[k], g[k][0]);
          assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, g[k][1]);
        };
        resolve();
      });
    });
  });
});
