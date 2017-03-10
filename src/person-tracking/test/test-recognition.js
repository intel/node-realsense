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

describe('Person Tracking Test Suite - recognition', function() {
  let options = {
    tracking: {
      enable: true,
    },
    recognition: {
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

  it('register', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(5000);
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        tracker.on('persontracked', (result) => {
          if (result.persons.length) {
            tracker.personRecognition.registerPerson(result.persons[0].trackInfo.id).then(
                (registerResult) => {
              assert.equal(registerResult.trackID, result.persons[0].trackInfo.id);
              return tracker.personRecognition.recognitionIDExist(registerResult.recognitionID);
            }).then((exist) => {
              assert.equal(exist, true);
              resolve();
            }).catch((e) => {
              // Don't reject, as for some frames, we may not be able to register.
            });
          }
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('register + recognize', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(10000);
    let registerResult;
    let recognitionResult;
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        tracker.on('persontracked', function(result) {
          if (result.persons.length) {
            tracker.personRecognition.registerPerson(result.persons[0].trackInfo.id).then(
                function(registerInfo) {
              registerResult = registerInfo;
              return tracker.personRecognition.recognizePerson(registerResult.trackID);
            }).then(function(recognizeInfo) {
              recognitionResult = recognizeInfo;
              assert.equal(recognitionResult.recognized, true);
              assert.equal(recognitionResult.trackID, registerResult.trackID);
              assert.equal(recognitionResult.recognitionID, registerResult.recognitionID);
              resolve();
            }).catch((e) => {
              // Don't reject, as for some frames, we may not be able to register.
            });
          }
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('register + recognizeAll', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(10000);
    let registerResult;
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        tracker.on('persontracked', function(result) {
          if (result.persons.length) {
            tracker.personRecognition.registerPerson(result.persons[0].trackInfo.id).then(
                function(registerInfo) {
              registerResult = registerInfo;
              return tracker.personRecognition.recognizeAllPersons();
            }).then(function(allinfo) {
              assert.equal(allinfo.length, 1);
              assert.equal(allinfo[0].data.recognized, true);
              assert.equal(allinfo[0].data.trackID, registerResult.trackID);
              assert.equal(allinfo[0].data.recognitionID, registerResult.recognitionID);
              assert.equal(allinfo[0].status, 'recognized');
              resolve();
            }).catch((e) => {
              // Don't reject, as for some frames, we may not be able to register.
            });
          }
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('register + unregister', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(10000);
    let registerResult;
    let registered = false;
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        tracker.on('persontracked', function(result) {
          if (registered)
            return;
          if (result.persons.length) {
            tracker.personRecognition.registerPerson(result.persons[0].trackInfo.id).then(
                function(registerInfo) {
              registerResult = registerInfo;
              registered = true;
              return tracker.personRecognition.getAllRecognitionIDs();
            }).then(function(ids) {
              assert.equal(ids.length, 1);
              assert.equal(ids[0], registerResult.recognitionID);
              return tracker.personRecognition.unRegisterPerson(ids[0]);
            }).then(function() {
              return tracker.personRecognition.getAllRecognitionIDs();
            }).then(function(newids) {
              assert.equal(newids.length, 0);
              resolve();
            }).catch((e) => {
              // Don't reject, as for some frames, we may not be able to register.
            });
          }
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('register + get and remove descriptor', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(10000);
    let registerResult;
    let registered = false;
    return new Promise(function(resolve, reject) {
      addon.createPersonTracker(options, cameraOptionsFromFile).then(function(inst) {
        tracker = inst;
        tracker.on('persontracked', function(result) {
          if (registered)
            return;
          if (result.persons.length) {
            tracker.personRecognition.registerPerson(result.persons[0].trackInfo.id).then(
                function(registerInfo) {
              registerResult = registerInfo;
              registered = true;
              return tracker.personRecognition.getPersonDescriptorIDs(registerResult.recognitionID);
            }).then(function(ids) {
              assert.equal(ids.length, 1);
              assert.equal(ids[0], registerResult.descriptorID);
              return tracker.personRecognition.removePersonDescriptor(
                  registerResult.recognitionID, ids[0]);
            }).then(function() {
              return tracker.personRecognition.getPersonDescriptorIDs(registerResult.recognitionID);
            }).then(function(newids) {
              assert.equal(newids.length, 0);
              resolve();
            }).catch((e) => {
              // Don't reject, as for some frames, we may not be able to register.
            });
          }
        });
        return tracker.start();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
});
