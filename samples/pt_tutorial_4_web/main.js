// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

let os = require('os');
let jpeg = require('jpeg-turbo');
let express = require('express');
let app = express();
let server = require('http').createServer(app);
let WsServer = require('ws').Server;
let ptModule = require('node-person');
let fs = require('fs');

let dbPath = './person_recognition_id.db';
let ptConfig = {
  tracking: {
    enable: true,
    trackingMode: 'following',
  },
  recognition: {
    enable: true,
  },
};
let cameraConfig = {color: {width: 320, height: 240, frameRate: 30, isEnabled: true},
                    depth: {width: 320, height: 240, frameRate: 30, isEnabled: true}};
let pt;
ptModule.createPersonTracker(ptConfig, cameraConfig).then((instance) => {
  pt = instance;
  startServer();

  pt.on('frameprocessed', function(result) {
    printPersonCount(result);
    pt.getFrameData().then((frame) => {
      sendRgbFrame(frame);
    });
  });
  pt.on('persontracked', function(result) {
    sendTrackingAndRecognitionData(result);
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

function constructAPersonData(person, recognitionID) {
  let trackInfo = person.trackInfo;
  if (trackInfo) {
    let element = {};
    let boundingBox = trackInfo.boundingBox;
    let center = trackInfo.center;
    if (recognitionID)
      element.rid = recognitionID;
    element.pid = trackInfo.id;
      element.cumulative_total = totalPersonIncrements;
    if (boundingBox) {
      element.person_bounding_box = {
        x: boundingBox.rect.x,
        y: boundingBox.rect.y,
        w: boundingBox.rect.width,
        h: boundingBox.rect.height,
      };
    }
    if (center) {
      element.center_mass_image = {
        x: center.imageCoordinate.x,
        y: center.imageCoordinate.x,
      };
      element.center_mass_world = {
        x: center.worldCoordinate.x,
        y: center.worldCoordinate.y,
        z: center.worldCoordinate.z,
      };
    }
    return element;
  }

  return undefined;
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
  let newIdSet = new Set();

  persons.forEach(function(person) {
    newIdSet.add(person.trackInfo.id);
  });

  numPeopleInFrame = persons.length;

  if (numPeopleInFrame > lastPersonCount)
    totalPersonIncrements += (numPeopleInFrame - lastPersonCount);
  else if (numPeopleInFrame === lastPersonCount && idSet !== null) {
    let diff = new Set(Array.from(idSet).filter((x) => !newIdSet.has(x)));
    totalPersonIncrements += diff.size;
  }

  idSet = newIdSet;

  lastPersonCount = numPeopleInFrame;

  if (numPeopleInFrame !== prevPeopleInFrame || totalPersonIncrements !== prevPeopleTotal) {
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

function sendTrackingAndRecognitionData(result) {
  if (!connected) {
    return;
  }
  let persons = result.persons;
  let resultArray = [];
  let promises = [];
  persons.forEach(function(person) {
    let element;
    let trackInfo = person.trackInfo;
    if (trackInfo) {
      let aPromise = new Promise((resolve, reject) => {
        pt.personRecognition.registerPerson(trackInfo.id).then((regData) => {
          console.log('Registered person: ', regData.recognitionID);
          element = constructAPersonData(person, regData.recognitionID);
          resultArray.push(element);
          resolve();
        }).catch((e) => {
          if (e.status === 'already-registered') {
            element = constructAPersonData(person, e.recognitionID);
          } else {
            element = constructAPersonData(person, undefined);
          }
          resultArray.push(element);
          resolve();
        });
      });
      promises.push(aPromise);
    }
  });

  Promise.all(promises).then(() => {
    let resultToDisplay = {
      Object_result: resultArray,
      type: 'person_tracking',
    };
    sendData(JSON.stringify(resultToDisplay));
  }).catch((e) => {
  });
}

function sendRgbFrame(frame) {
  if (!connected) {
    return;
  }
  let useJpeg = true;
  let width = frame.color.width;
  let height = frame.color.height;
  let rawData = frame.color.data;

  let imageBuffer;
  let imageBufferLength;
  if (useJpeg) {
    imageBuffer = encodeToJPEG(rawData, width, height);
    imageBufferLength = imageBuffer.byteLength;
  } else {
    imageBuffer = rawData;
    imageBufferLength = rawData.length;
  }

  const msgHeaderLength = 16;
  let msg = new ArrayBuffer(msgHeaderLength + imageBufferLength);
  let int8View = new Uint8Array(msg);
  int8View.set(imageBuffer, msgHeaderLength);

  let int16View = new Uint16Array(msg, 0, msgHeaderLength);
  const MSG_RGB = 3;
  const FrameFormat = {
    Raw: 0,
    Jpeg: 1,
  };

  // The schema of the sent message:
  // |type|format|width|height|padding|time|data|
  // type: 1 byte, 3 means RGB frame data
  // format: 1 byte, 0 means raw data with out encoding, 1 means jpeg
  // width: 2 bytes, width of the frame data
  // height: 2 bytes, height of the frame data
  // padding: 2 bytes
  // time: 8 bytes, time stamp, not used currently.
  // data: the RGB data.
  int8View[0] = MSG_RGB;  // type
  if (useJpeg)
    int8View[1] = FrameFormat.Jpeg;  // format, jpeg
  else
    int8View[1] = FrameFormat.Raw;  // format, raw
  int16View[1] = width;
  int16View[2] = height;
  int16View[3] = 0;  // padding

  sendData(msg);
}

let clients = [];
let connected = false;

function sendData(data) {
  if (clients.length !== 0) {
    try {
      clients.forEach((client) => {
        client.send(data);
      });
    } catch (exception) {
      console.log('Exception: send data failed exception:', exception);
    }
  }
}

function encodeToJPEG(buffer, width, height) {
  let options = {
    format: jpeg.FORMAT_RGB,
    width: width,
    height: height,
    quality: 80,
  };
  let jpegImageData = jpeg.compressSync(buffer, options);
  return jpegImageData;
}

function getEthernetIp() {
  let ifaces = os.networkInterfaces();
  let ip = '';
  for (let ifname in ifaces) {
    if (ifname === undefined)
      continue;
    ifaces[ifname].forEach(function(iface) {
      if ('IPv4' !== iface.family || iface.internal !== false) {
        return;
      }
      ip = iface.address;
    });
    if (ip !== '')
      return ip;
  }
  return ip;
}

function saveDatabase() {
  pt.personRecognition.exportDatabase().then((db) => {
    fs.writeFile(dbPath, db, (err) => {
      if (err)
        console.log('write to ' + dbPath + ' failed: ' + err);
    });
  }).catch((e) => {
    console.log(e);
  })
}

function loadDatabase() {
  fs.readFile(dbPath, (err, data) => {
    if (err) {
      console.log('failed to open database file');
      return;
    }
    // console.log('----------- load db successed, data.buffer', data.buffer);
    pt.personRecognition.importDatabase(data).then(() => {
    }).catch((error) => {
      console.log('failed to load database: ' + error);
    });
  });
}

function startServer() {
  // Share the ui-browser code from cpp sample
  app.use(express.static('./ui-browser/src'));
  const ip = getEthernetIp();
  const port = 8000;
  server.listen(port, ip);
  let wss = new WsServer({
    server: server,
  });

  console.log('\nEthernet ip:' + ip);
  console.log(' >>> point your browser to: http://' + ip + ':' + port + '/view.html');

  wss.on('connection', function(client) {
    console.log('server: got connection ' + client._socket.remoteAddress + ':' +
        client._socket.remotePort);
    clients.push(client);
    if (!connected)
      connected = true;
    client.on('close', function() {
      console.log('server: disconnect ' + client._socket.remoteAddress + ':' +
          client._socket.remotePort);
      let index = clients.indexOf(client);
      if (index > -1) {
        clients.splice(index, 1);
      }
      if (clients.length === 0)
        connected = false;
    });
    client.on('message', function(msg) {
      let msgobj;
      try {
        msgobj = JSON.parse(msg);
      } catch (e) {
        // console.log(e);
        return;
      }
      if (msgobj.type === 'pt_track') {
        let id = parseInt(msgobj.command);
        if (isNaN(id)) {
          console.log('pt_tutorial_4_web main:  Invalid person ID ', msgobj.command);
          return;
        } else {
          let work;
          if (id > -1) {
            work =pt.personTracking.startTrackingPerson(id);
          } else {
            work = pt.personTracking.resetTracking();
          }
          work.then(() => {
            return saveDatabase();
          }).catch(() => {
          });
        }
      } else if (msgobj.type === 'control') {
        if (msgobj.command === 'load_pt_db') {
          loadDatabase();
        }
      }
    });
  });
}
