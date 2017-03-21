// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

let pt = require('..');
let trackerOptions = {
  skeleton: {
    enable: true,
  },
  tracking: {
    enable: true,
    enableHeadBoundingBox: true,
  },
  recognition: {
    enable: true,
    policy: 'standard',
    useMultiFrame: false,
  },
  gesture: {
    enable: true,
    enableAllGestures: true,
  },
  personFace: {
    enableFaceLandmarks: true,
    enableHeadPose: true,
  },
};

let cameraOptions = {
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
};

let tracker = null;
let recogCtl = null;
let registerOngoing = false;
let registerPerformed = false;
let intervalId = null;
pt.createPersonTracker(trackerOptions, cameraOptions).then((personTracker) => {
  tracker = personTracker;
  recogCtl = tracker.faceRecognition;
  intervalId = setInterval(function() {
    console.log('------------------ state: ', tracker.state);
  }, 500);

  operate('start', 5000, 'Start');
  operate('stop', 15000, 'Stop');
}).catch((e) => {
  console.log('Failed to start, ', e);
  clearInterval(intervalId);
});

function registerTestWork() {
  tracker.on('persontracked', function(result) {
    result.persons.forEach(function(person) {
      skeletonTest(person);
      trackingTest(person);
      personFaceTest(person);
      gestureTest(person);
      // Comment register test as it's not formally supported yet.
      // registerAndReinforceTest(person);
    });
  });
}
// eslint-disable-next-line
function registerAndReinforceTest(person) {
  console.log('-- registration result begin');
  let ids = null;
  if (person.trackInfo) {
    if (registerPerformed === false) {
      if (registerOngoing === false) {
        registerOngoing = true;
        recogCtl.registerPerson(person.trackInfo.id).then(function(recogID) {
          console.log('--register person success track ID, recog ID:',
              person.trackInfo.id, recogID);
          registerPerformed = true;
          return recogCtl.reinforceRegistration(person.trackInfo.id, recogID, 'manual-add');
        }).then(function() {
          console.log('--reinforce person success');
        }).then(function() {
          return recogCtl.getRegisteredIDs();
        }).then(function(theIds) {
          console.log('--registered IDS:', theIds);
          ids = theIds;
          return recogCtl.isPersonRegistered(ids[0]);
        }).then(function(exist) {
          console.log('--ids[0] registered? :', exist);
          console.log('--unRegister ids[0]:', ids[0]);
          return recogCtl.unRegisterPerson(ids[0]);
        }).then(function() {
          console.log('--ids[0] unregistered:', ids[0]);
        }).then(function() {
          return recogCtl.getRegisteredIDs();
        }).then(function(newids) {
          console.log('--final registered ids: ', newids);
          registerOngoing = false;
        }).catch(function(err) {
          console.log('--register person test failed, track ID:' + person.trackInfo.id +
              ' failed: ' + err);
          registerOngoing = false;
        });
      }
    }
  }
  console.log('-- registration result end');
}

function skeletonTest(person) {
  console.log('-- skeleton result begin');
  if (person.skeletonInfo) {
    console.log('-- skeleton:');
    person.skeletonInfo.skeletonJoints.forEach(function(point) {
      console.log(point);
    });
  }
  console.log('-- skeleton result end');
}

function gestureTest(person) {
  console.log('-- gesture result begin');
  let gesture = person.gestureInfo;
  if (!gesture)
    return;
  let info = gesture.pointingInfo;
  if (gesture.isPointing && info) {
    console.log('--gesture: pointingInfo: colorOrigin(x,y), direction(x,y), confidence,' +
        'timestamp: ', info.colorPointingData.origin.x, info.colorPointingData.origin.y,
        info.colorPointingData.direction.x, info.colorPointingData.direction.y,
        info.confidence, info.gestureStartTimeStamp);
    console.log('--gesture: pointingInfo: worldOrigin(x,y,z), direction(x,y,z)',
        info.worldPointingData.origin.x, info.worldPointingData.origin.y,
        info.worldPointingData.origin.z, info.worldPointingData.direction.x,
        info.worldPointingData.direction.y, info.worldPointingData.direction.z);
  }
  console.log('-- gesture result end');
}

function personFaceTest(person) {
  console.log('-- personFace result begin');
  let face = person.faceInfo;
  if (!face)
    return;
  let pose = face.headPose;
  let markInfo = face.landmarkInfo;

  if (pose) {
    console.log('--head pose: yaw, pitch, roll:', pose.yaw, pose.pitch, pose.roll);
  }
  if (markInfo) {
    let mark = markInfo.landmarks[0];
    console.log('--landmark: confidence:' + markInfo.confidence + ' markcount: ' +
        markInfo.landmarks.length);
    console.log('--mark[0]:' + mark.imageCoordinate.x + ',' + mark.imageCoordinate.y);
  }
  console.log('-- personFace result end');
}

function trackingTest(person) {
  console.log('-- tracking result begin');
  let info = person.trackInfo;
  if (!info)
    return;
  let box = info.boundingBox;
  let hbox = info.headBoundingBox;
  let center = info.center;
  let seg = info.segmentationMask;

  tracker.personTracking.startTrackingPerson(person.trackInfo.id).then(function() {
    console.log('-- successfully started tracking person');
  }).catch(function(e) {
    console.log('-- failed tracking person');
  });

  console.log('--id:' + info.id);
  if (box) {
    console.log('--box: confidence: ' + box.confidence + ', x: ' + box.rect.x +
        ', y: ' + box.rect.y + ', w: ' + box.rect.width + ', h: ' + box.rect.height);
  }
  if (hbox) {
    console.log('--headbox: confidence: ' + hbox.confidence + ', x: ' + hbox.rect.x + ', y: ' +
        hbox.rect.y + ', w: ' + hbox.rect.width + ', h: ' + hbox.rect.height);
  }
  if (center) {
    console.log('--center: imageCoordinate and confidence:', center.imageCoordinate.x,
        center.imageCoordinate.y, center.imageConfidence);
    console.log('--center: worldCoordinate and confidence:', center.worldCoordinate.x,
        center.worldCoordinate.y, center.worldCoordinate.z, center.worldConfidence);
  }
  if (seg) {
    console.log('--segmentation: width/height', seg.width, seg.height, seg.maskData);
  }
  console.log('-- tracking result end');
}

function operate(op, time, des) {
  setTimeout(function() {
    console.log('------------', des);
  }, time);
  // eslint-disable-next-line
  let theArgs = arguments;
  setTimeout(function() {
    if (op === 'start') {
      registerTestWork();
    } else if (op === 'stop') {
      clearInterval(intervalId);
    }
    // eslint-disable-next-line
    tracker[op].apply(tracker, Array.prototype.slice.call(theArgs, 3)).then(function() {
      console.log('------------ ', op, ' done');
    }).catch(function(e) {
      console.log('------------', op, ' failed');
    });
  }, time + 500);
}
