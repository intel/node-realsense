// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('..');

function runLocalization() {
  const options = {
    mode: 'localization',
    confidenceThreshold: 0.75,
    computeEngine: 'CPU',
    enableSegmentation: true,
    enableObjectCenterEstimation: true,
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
      or.on('localization', function(eventData) {
        console.log('New [localization] event: ', eventData.length);
        eventData.forEach((item) => {
          console.log(item.label, item.probability, item.roi.toString(), item.objectCenter);
        });
        console.log('----');
      });

      or.start();
    }).catch((e) => {
      console.log(`Something was wrong ${e}`);
    });
  });
}

runLocalization();
