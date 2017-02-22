// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

/* global describe, it */
const assert = require('assert');
const addon = require('bindings')('realsense_object_recognition');

function testCameraOptionsP(data) {
  describe('Object Recognition Test Suite - Options - Positive', function() {
    it('Validate options sent to createObjectRecognizer() - color:'
      + data.color.width + 'x' + data.color.height + ' - depth:'
      + data.depth.width + 'x' + data.depth.height, function() {
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer(null, cameraOptions).then((objectRecognizer) => {
          return objectRecognizer.getCameraOptions();
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
  });
}

function testsetCameraOptionsP(data) {
  describe('Object Recognition Test Suite - Options - Positive', function() {
    it('Validate options sent to setCameraOptions() - color:'
      + data.color.width + 'x' + data.color.height + ' - depth:'
      + data.depth.width + 'x' + data.depth.height, function() {
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then((objectRecognizer) => {
          return objectRecognizer.setCameraOptions(cameraOptions)
          .then(function() {
            return objectRecognizer.getCameraOptions();
          });
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
  });
}

function testCameraOptionsN(data) {
  describe('Object Recognition Test Suite - Options - Negative', function() {
    it('Validate options sent to createObjectRecognizer() - color:'
      + data.color.width + 'x' + data.color.height + ' - color frameRate:'
      + data.color.frameRate + ' - depth:'
      + data.depth.width + 'x' + data.depth.height + ' - depth frameRate:'
      + data.depth.frameRate, function() {
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer(null, cameraOptions).then((objectRecognizer) => {
          return objectRecognizer.getCameraOptions();
        }).then((options)=> {
          reject('should fail but promise got resolved');
        }).catch((e) => {
          resolve();
        });
      });
    });
  });
}

function testsetCameraOptionsN(data) {
  describe('Object Recognition Test Suite - setCameraOptions() - Negative', function() {
    it('Validate options sent to setCameraOptions() - color:'
      + data.color.width + 'x' + data.color.height + ' - color frameRate:'
      + data.color.frameRate + ' - depth:'
      + data.depth.width + 'x' + data.depth.height + ' - depth frameRate:'
      + data.depth.frameRate, function() {
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      return new Promise((resolve, reject) => {
        addon.createObjectRecognizer().then((objectRecognizer) => {
          return objectRecognizer.setCameraOptions(cameraOptions)
          .then(function() {
            return objectRecognizer.getCamerOptions();
          });
        }).then((options)=> {
          reject('should fail but promise got resolved');
        }).catch((e) => {
          resolve();
        });
      });
    });
  });
}
let arrPositive = [
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 480, height: 360, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 640, height: 480, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
];

let arrNegative = [
  {
    color: {width: 30, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 300, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 700, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 123, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 1},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 1},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 50},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 50},
  },
  {
    color: {width: '640', height: 480, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
];

// eslint-disable-next-line guard-for-in
for(let i in arrPositive) {
  testCameraOptionsP(arrPositive[i]);
  testsetCameraOptionsP(arrPositive[i]);
}

// eslint-disable-next-line guard-for-in
for(let j in arrNegative) {
  testCameraOptionsN(arrNegative[j]);
  testsetCameraOptionsN(arrNegative[j]);
}
