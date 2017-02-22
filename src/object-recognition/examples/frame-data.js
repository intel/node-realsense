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

  addon.createObjectRecognizer().then((objectRecognizer) => {
    let or = objectRecognizer;
    or.setCameraOptions({
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
      return or.setObjectRecognitionOptions(options);
    }).then(() => {
      or.on('recognition', function(eventData) {
        console.log('[recognition] event, objects =', eventData.length);
        eventData.forEach((item) => {
          console.log(item.label, '|', item.probability);
        });
        console.log('----');

        or.getFrameData().then(function(info) {
          console.log(info);
        });
      });

      or.start();
    }).catch((e) => {
      console.log(`Something was wrong ${e}`);
    });
  });
}

runObjectRecognition();
