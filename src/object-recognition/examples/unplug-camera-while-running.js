// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('..');

function runObjectRecognition() {
  const options = {
    mode: 'single-recognition',
    confidenceThreshold: 0.35,
    computeEngine: 'CPU',
    enableSegmentation: true,
    maxReturnObjectCount: 2,
    enableObjectCenterEstimation: false,
  };

  addon.createObjectRecognizer().then((inst) => {
    let instance = inst;
    inst.setCameraOptions({
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
    }).then(() => {
      return instance.setObjectRecognitionOptions(options);
    }).then(() => {
      instance.on('recognition', function(eventData) {
        process.stdout.write('.');
      });

      instance.start();
    }).catch((e) => {
      console.log(`Something was wrong ${e}`);
    });
  });
}

process.on('uncaughtException', (err) => {
  console.log(`uncaughtException: ${er}`);
});

runObjectRecognition();
