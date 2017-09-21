// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

const orModule = require('node-object');

let or;
let mode = 'localization';
let objectsNames = [];
const options = {
  mode: mode,
  confidenceThreshold: 0.7,
};

orModule.createObjectRecognizer().then((objectRecognizer) => {
  or = objectRecognizer;
  or.on('localization', (results) => {
    if (mode === 'localization') {
      drawLocalizationResults(results);
      setTracking(results);
    }
  });
  or.on('tracking', (results) => {
    if (mode === 'tracking')
      drawTrackingResults(results);
  });

  return or.setObjectRecognitionOptions(options);
}).then(() => {
  return or.start();
}).catch((e) => {
  console.log('error: ' + e);
});

function drawLocalizationResults(results) {
  const w = 18;
  console.log('localization object information:');
  console.log(padding('Label', w) + padding('Confidence', w) +
      padding('Object Center', 28) + padding('Coordinate', w));
  console.log(padding('-----', w) + padding('----------', w) +
      padding('---------------', 28) + padding('------------', w));

  results.forEach((localizationData) => {
    let objName = localizationData.label;
    objectsNames.push(objName);
    let confidence = localizationData.probability;
    let center = localizationData.objectCenter;
    let rect = localizationData.roi;
    console.log(padding(objName, w) +
      padding(confidence.toPrecision(2).toString(), w) +
      padding(`(${center.x.toPrecision(2)}, ` +
              `${center.y.toPrecision(2)}, ` +
              `${center.z.toPrecision(2)})`, 28) +
      padding(`(${rect.x}, ${rect.y}) (${rect.x+rect.width}, ${rect.y+rect.height})`, w));
  });
  console.log('Will track all of these objects.');
  console.log('');
}

function setTracking(results) {
  mode = 'tracking';
  let trackingOptions = {
    mode: mode,
    trackingRois: [],
  };

  results.forEach((localizationData) => {
    let roi = localizationData.roi;
    trackingOptions.trackingRois.push(new orModule.Rect2D(roi.x, roi.y, roi.width, roi.height));
  });

  or.setObjectRecognitionOptions(trackingOptions).catch((e) => {
    console.log('error: ' + e);
  });
}

function drawTrackingResults(results) {
  const w = 18;
  console.log(padding('Label', w) + padding('Coordinate', w));
  console.log(padding('-----', w) + padding('------------', w));

  let i = 0;
  results.forEach((trackingData) => {
    let roi = trackingData.roi;
    console.log(padding(objectsNames[i++], w) +
      `(${roi.x}, ${roi.y}) (${roi.x+roi.width}, ${roi.y+roi.height})`);
  });
  console.log('');
}

console.log('\n-------- Press Esc key to exit --------\n');

const ESC_KEY = '\u001b';
const CTRL_C = '\u0003';
let stdin = process.stdin;
stdin.setRawMode(true);
stdin.resume();
stdin.setEncoding('utf8');
stdin.on('data', function(key) {
  if (key === ESC_KEY || key === CTRL_C) {
    exit();
  }
});

function exit() {
  console.log('\n-------- Stopping --------');
  if (or) {
    or.stop().then(() => {
      process.exit();
    }).catch((error) => {
      console.log('error: ' + error);
      process.exit();
    });
  } else {
    process.exit();
  }
}

function padding(string, width) {
  if (!(string instanceof String))
    string = String(string);
  let length = width - string.length;
  if (length <= 0) return string;
  return string + new Array(length + 1).join(' ');
}
