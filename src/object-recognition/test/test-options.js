// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');

describe('Object Recognition Test Suite - Options', function() {
  // eslint-disable-next-line max-len
  it('Validate options sent to setObjectRecognitionOptions() by getting them back from getObjectRecognitionOptions()', function() {
    return new Promise((resolve, reject) => {
      let or = undefined;
      const trackingOptions = {
          mode: 'tracking',
          confidenceThreshold: 0.95,
          computeEngine: 'GPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: true,
        };

      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setObjectRecognitionOptions(trackingOptions);
      }).then(() => {
        return or.getObjectRecognitionOptions();
      }).then((options) => {
        assert.equal(options.mode, trackingOptions.mode);
        assert.equal(options.confidenceThreshold, trackingOptions.confidenceThreshold);
        assert.equal(options.computeEngine, trackingOptions.computeEngine);
        assert.equal(options.enableSegmentation, trackingOptions.enableSegmentation);
        assert.equal(options.maxReturnObjectCount, trackingOptions.maxReturnObjectCount);
        assert.equal(options.enableObjectCenterEstimation,
          trackingOptions.enableObjectCenterEstimation);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('getCameraOptions() should return meaningful data by default', function() {
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        return objectRecognizer.getCameraOptions();
      }).then((options)=> {
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
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('setCameraOptions() can work', function() {
    let or;
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        return or.setCameraOptions({
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
        });
      }).then(()=> {
        return or.setCameraOptions();
      }).then(()=> {
        return or.setCameraOptions({});
      }).then(()=> {
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Wrong options should be rejected by setCameraOptions()', function() {
    let or;
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.setCameraOptions({
          color: {
            width: 3840,
            height: 2160,
            frameRate: 120,
            isEnabled: true,
          },
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        reject('Should not accept 4x FullHD resolution options for color channel');
      }).catch((e) => {
        resolve();
      });
    });
  });

  // eslint-disable-next-line max-len
  it('setCameraOptions() should not overwrite members that are not present in dictionary', function() {
    let previousOptions;
    let or;
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.getCameraOptions();
      }).then((options)=> {
        previousOptions = options;
        // Change only depth channel options
        return or.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        return or.getCameraOptions();
      }).then((options)=> {
        // Check whether color channel options are not changed
        assert.equal(previousOptions.color.width, options.color.width);
        assert.equal(previousOptions.color.height, options.color.height);
        assert.equal(previousOptions.color.frameRate, options.color.frameRate);
        assert.equal(previousOptions.color.isEnabled, options.color.isEnabled);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  // eslint-disable-next-line max-len
  it('Validate options sent to setCameraOptions() by getting them back from getCameraOptions()', function() {
    const myOptions = {
      color: {
        width: 1920,
        height: 1080,
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
    let or;
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer().then((objectRecognizer) => {
        or = objectRecognizer;
        return or.getCameraOptions();
      }).then((options)=> {
        assert.notEqual(myOptions.color.width, options.color.width);
        assert.notEqual(myOptions.color.height, options.color.height);
        assert.notEqual(myOptions.depth.width, options.depth.width);
        assert.notEqual(myOptions.depth.height, options.depth.height);
        return or.setCameraOptions(myOptions);
      }).then(()=> {
        return or.getCameraOptions();
      }).then((options)=> {
        // Check whether color channel options are not changed
        assert.equal(myOptions.color.width, options.color.width);
        assert.equal(myOptions.color.height, options.color.height);
        assert.equal(myOptions.color.frameRate, options.color.frameRate);
        assert.equal(myOptions.color.isEnabled, options.color.isEnabled);
        assert.equal(myOptions.depth.width, options.depth.width);
        assert.equal(myOptions.depth.height, options.depth.height);
        assert.equal(myOptions.depth.frameRate, options.depth.frameRate);
        assert.equal(myOptions.depth.isEnabled, options.depth.isEnabled);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  // eslint-disable-next-line max-len
  it('Validate options sent to createObjectRecognizer() by getting them back from getObjectRecognitionOptions()', function() {
    return new Promise((resolve, reject) => {
      const trackingOptions = {
          mode: 'tracking',
          confidenceThreshold: 0.95,
          computeEngine: 'GPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: true,
        };

      addon.createObjectRecognizer(trackingOptions).then((or) => {
        return or.getObjectRecognitionOptions();
      }).then((options) => {
        assert.equal(options.mode, trackingOptions.mode);
        assert.equal(options.confidenceThreshold, trackingOptions.confidenceThreshold);
        assert.equal(options.computeEngine, trackingOptions.computeEngine);
        assert.equal(options.enableSegmentation, trackingOptions.enableSegmentation);
        assert.equal(options.maxReturnObjectCount, trackingOptions.maxReturnObjectCount);
        assert.equal(options.enableObjectCenterEstimation,
          trackingOptions.enableObjectCenterEstimation);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  // eslint-disable-next-line max-len
  it('Validate options sent to createObjectRecognizer() by getting them back from getCameraOptions()', function() {
    const cameraOptions = {
      color: {
        width: 1920,
        height: 1080,
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
    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(null, cameraOptions).then((or) => {
        return or.getCameraOptions();
      }).then((options)=> {
        // Check whether color channel options are not changed
        assert.equal(cameraOptions.color.width, options.color.width);
        assert.equal(cameraOptions.color.height, options.color.height);
        assert.equal(cameraOptions.color.frameRate, options.color.frameRate);
        assert.equal(cameraOptions.color.isEnabled, options.color.isEnabled);
        assert.equal(cameraOptions.depth.width, options.depth.width);
        assert.equal(cameraOptions.depth.height, options.depth.height);
        assert.equal(cameraOptions.depth.frameRate, options.depth.frameRate);
        assert.equal(cameraOptions.depth.isEnabled, options.depth.isEnabled);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Wrong CameraOptions sent to createObjectRecognizer() should be rejected (1)', function() {
    const wrongCameraOptions = {
      color: {
        width: 1920,
        height: 1080,
        frameRate: 30,
        isEnabled: true,
      },
      depth: {
        width: 3840,  // Wrong width
        height: 468,
        frameRate: 30,
        isEnabled: true,
      },
    };

    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(null, wrongCameraOptions).then((objectRecognizer) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  it('Wrong CameraOptions sent to createObjectRecognizer() should be rejected (2)', function() {
    const wrongCameraOptions = {
      color: {
        width: 640,
        height: 480,
        frameRate: 60,  // Unsupported FPS rate
        isEnabled: true,
      },
    };

    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(null, wrongCameraOptions).then((objectRecognizer) => {
        reject('should fail but promise got resolved');
      }).catch((e) => {
        resolve('success');
      });
    });
  });

  // eslint-disable-next-line max-len
  it('Wrong ObjectRecognitionOptions sent to createObjectRecognizer() should have no effect', function() {
    const wrongObjectRecognitionOptions = {
      Mode: 'tracking',
      confidencethreshold: 0.95,
      color: {width: 640, height: 480, frameRate: 60, isEnabled: true}};

    return new Promise((resolve, reject) => {
      addon.createObjectRecognizer(wrongObjectRecognitionOptions).then((objectRecognizer) => {
        return objectRecognizer.getObjectRecognitionOptions();
      }).then((options)=> {
        assert.equal(typeof options.color, 'undefined');
        assert.equal(typeof options.Mode, 'undefined');
        assert.equal(typeof options.confidencethreshold, 'undefined');
        resolve('success');
      }).catch((e) => {
        reject('should not fail but promise got rejected with: ' + e);
      });
    });
  });

  // eslint-disable-next-line max-len
  it('Validate options sent to createObjectRecognizer() by getting them back from getObjectRecognitionOptions() and getCameraOptions()', function() {
    return new Promise((resolve, reject) => {
      const trackingOptions = {
          mode: 'tracking',
          confidenceThreshold: 0.95,
          computeEngine: 'GPU',
          enableSegmentation: true,
          maxReturnObjectCount: 3,
          enableObjectCenterEstimation: true,
        };
      const cameraOptions = {
        color: {
          width: 1920,
          height: 1080,
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

      addon.createObjectRecognizer(trackingOptions, cameraOptions).then((or) => {
        return or.getObjectRecognitionOptions()
        .then(function(options) {
          assert.equal(options.mode, trackingOptions.mode);
          assert.equal(options.confidenceThreshold, trackingOptions.confidenceThreshold);
          assert.equal(options.computeEngine, trackingOptions.computeEngine);
          assert.equal(options.enableSegmentation, trackingOptions.enableSegmentation);
          assert.equal(options.maxReturnObjectCount, trackingOptions.maxReturnObjectCount);
          assert.equal(options.enableObjectCenterEstimation,
            trackingOptions.enableObjectCenterEstimation);
          return or.getCameraOptions();
        })
        .then(function(options) {
          assert.equal(cameraOptions.color.width, options.color.width);
          assert.equal(cameraOptions.color.height, options.color.height);
          assert.equal(cameraOptions.color.frameRate, options.color.frameRate);
          assert.equal(cameraOptions.color.isEnabled, options.color.isEnabled);
          assert.equal(cameraOptions.depth.width, options.depth.width);
          assert.equal(cameraOptions.depth.height, options.depth.height);
          assert.equal(cameraOptions.depth.frameRate, options.depth.frameRate);
          assert.equal(cameraOptions.depth.isEnabled, options.depth.isEnabled);
          resolve();
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
});
