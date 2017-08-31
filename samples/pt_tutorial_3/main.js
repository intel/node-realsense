// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

let ptModule = require('node-person');

let ptConfig = {
  tracking: {
    enable: true,
  },
  gesture: {
    enable: true,
    enableAllGestures: true,
  },
};
let cameraConfig = {color: {width: 320, height: 240, frameRate: 30, isEnabled: true},
                    depth: {width: 320, height: 240, frameRate: 30, isEnabled: true}};
let pt;

ptModule.createPersonTracker(ptConfig, cameraConfig).then((instance) => {
  pt = instance;
  console.log('Enabling Pointing gesture');

  pt.on('persontracked', function(result) {
    startTracking(result);
    printPointingGestureInfo(result);
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

let trackingPersonId;

function startTracking(result) {
  if (pt.state === 'detecting' && result.persons.length > 0) {
    // Start tracking the first person detected in the frame.
    console.log('Call StartTracking()');
    trackingPersonId = result.persons[0].trackInfo.id;
    pt.personTracking.startTrackingPerson(trackingPersonId);
  }
}

function printPointingGestureInfo(result) {
  if (result.persons.length > 0) {
    let personData;
    result.persons.forEach(function(person) {
      if (person.trackInfo.id === trackingPersonId)
        personData = person;
    });

    let personGestures = personData.gestureInfo;
    if (personGestures !== undefined) {
      if (personGestures.isPointing && personGestures.pointingInfo.confidence > 0) {
        console.log('Pointing detected, PID: ' + personData.trackInfo.id);
        console.log('    color coordinates: origin(x,y): ' +
            personGestures.pointingInfo.colorPointingData.origin.x + ', ' +
            personGestures.pointingInfo.colorPointingData.origin.y);
        console.log('                    direction(x,y): ' +
            personGestures.pointingInfo.colorPointingData.direction.x + ', ' +
            personGestures.pointingInfo.colorPointingData.direction.y);
        console.log('    world coordinates: origin(x,y,z): ' +
            personGestures.pointingInfo.worldPointingData.origin.x + ', ' +
            personGestures.pointingInfo.worldPointingData.origin.y + ', ' +
            personGestures.pointingInfo.worldPointingData.origin.z);
        console.log('                    direction(x,y,z): ' +
            personGestures.pointingInfo.worldPointingData.direction.x + ', ' +
            personGestures.pointingInfo.worldPointingData.direction.y + ', ' +
            personGestures.pointingInfo.worldPointingData.direction.z);
      }
    }
  }
}
