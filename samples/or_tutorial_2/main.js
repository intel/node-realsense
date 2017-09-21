// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

const orModule = require('node-object');

let or;
const options = {
  mode: 'localization',
  confidenceThreshold: 0.7,
  enableObjectCenterEstimation: true,
};

orModule.createObjectRecognizer().then((objectRecognizer) => {
  or = objectRecognizer;
  or.on('localization', (results) => {
    drawResults(results);
  });

  return or.setObjectRecognitionOptions(options);
}).then(() => {
  return or.start();
}).catch((e) => {
  console.log('error: ' + e);
});

function drawResults(results) {
  const w = 18;
  console.log(padding('Label', w) + padding('Confidence', w) +
      padding('Object Center', 28) + padding('Coordinate', w));
  console.log(padding('-----', w) + padding('----------', w) +
      padding('---------------', 28) + padding('------------', w));

  results.forEach((localizationData) => {
    let objName = localizationData.label;
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
