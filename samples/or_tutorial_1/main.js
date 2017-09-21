// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

const orModule = require('node-object');

let or;
const options = {
  framesSkipping: 50,
};

orModule.createObjectRecognizer().then((objectRecognizer) => {
  or = objectRecognizer;
  or.on('recognition', (results) => {
    drawResults(results);
  });

  return or.setObjectRecognitionOptions(options);
}).then(() => {
  return or.start();
}).then(() => {
  return or.getCameraOptions();
}).then((cameraOptions) => {
  let width = cameraOptions.color.width;
  let height = cameraOptions.color.height;
  let orOptions = {
    roi: new orModule.Rect2D(width/4, height/4, width/2, height/2),
  };
  return or.setObjectRecognitionOptions(orOptions);
}).catch((e) => {
  console.log('error: ' + e);
});


function drawResults(results) {
  if (results.length === 0)
    return;
  const w = 18;
  let recognitionData = results[0];
  console.log(padding('Label', w), padding('Confidence', w));
  console.log(padding('-----', w), padding('----------', w));
  console.log(padding(recognitionData.label, w),
              padding(recognitionData.probability.toPrecision(2).toString(), w));
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
