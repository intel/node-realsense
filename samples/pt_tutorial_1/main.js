// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

let ptModule = require('node-person');

let ptConfig = {tracking: {enable: true, trackingMode: 'following'}};
let cameraConfig = {color: {width: 320, height: 240, frameRate: 30, isEnabled: true},
                    depth: {width: 320, height: 240, frameRate: 30, isEnabled: true}};
let pt;
ptModule.createPersonTracker(ptConfig, cameraConfig).then((instance) => {
  pt = instance;
  console.log('Enabling Tracking with mode set to 0');

  pt.on('frameprocessed', function(result) {
    printPersonCount(result);
  });

  return pt.start();
}).catch((error) => {
  console.log('error: ' + error);
});

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
  if (pt) {
    pt.stop().then(() => {
      process.exit();
    }).catch((error) => {
      console.log('error: ' + error);
      process.exit();
    });
  } else {
    process.exit();
  }
}

let lastPersonCount = 0;
let totalPersonIncrements = 0;
let prevPeopleInFrame = 0;
let prevPeopleTotal = 0;
let idSet = null;
let w = 25;

function printPersonCount(result) {
  let persons = result.persons;
  let numPeopleInFrame = 0;
  let newIdSet = new Set;

  for (let i = 0; i < persons.length; ++i) {
    newIdSet.add(persons[i].trackInfo.id);
  }

  numPeopleInFrame = persons.length;

  if (numPeopleInFrame > lastPersonCount)
    totalPersonIncrements += (numPeopleInFrame - lastPersonCount);
  else if (numPeopleInFrame == lastPersonCount && idSet != null) {
    let diff = new Set(Array.from(idSet).filter((x) => !newIdSet.has(x)));
    totalPersonIncrements += diff.size;
  }

  idSet = newIdSet;

  lastPersonCount = numPeopleInFrame;

  if (numPeopleInFrame != prevPeopleInFrame || totalPersonIncrements != prevPeopleTotal) {
    console.log(padding('Current Frame Total', w), padding('Cumulative', w));
    console.log(padding('--------------------', w), padding('----------', w));
    console.log(padding(numPeopleInFrame, w), padding(totalPersonIncrements, w));

    prevPeopleInFrame = numPeopleInFrame;
    prevPeopleTotal = totalPersonIncrements;
  }
}

function padding(string, width) {
  if (!(string instanceof String))
    string = String(string);
  let length = width - string.length;
  if (length <= 0) return string;
  return string + new Array(length + 1).join(' ');
}
