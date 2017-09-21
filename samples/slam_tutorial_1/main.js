// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

let slamModule = require('node-slam');

let slam;
slamModule.createInstance().then((instance) => {
  slam = instance;
  
  let slamOptions = {
    occupancyMapResolution: 0.025,
  };
  return slam.setInstanceOptions(slamOptions);
}).then(() => {
  console.log('Starting SLAM...');
  slam.on('tracking', (result) => {
    printTrackingResult(result);
  });
  return slam.start();
}).catch((error) => {
  console.log('error: ' + error);
});

function printTrackingResult(result) {
  let pose = result.cameraPose;
  let accuracy = result.accuracy;
  slam.getOccupancyMapUpdate().then((mapData) => {
    let countUpdatedTiles = mapData.tileCount;
    console.log(`Translation:(X=${pose[3].toPrecision(2)}, Y=${pose[7].toPrecision(2)}, Z=${pose[11].toPrecision(2)})` +
        `    Accuracy: ${accuracy}    Tiles_Updated: ${countUpdatedTiles}`);
  });
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
  if (slam) {
    console.log('Saving occupancy map to disk...');
    slam.saveOccupancyMapAsPpm('occupancy.ppm', true).then(() => {
      console.log('Stopping...');
      return slam.stop();
    }).then(() => {
      process.exit();
    }).catch((error) => {
      console.log('error: ' + error);
      process.exit();
    })
  } else {
    process.exit();
  }
}
