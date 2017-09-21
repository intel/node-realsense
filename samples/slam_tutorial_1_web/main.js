// License: Apache 2.0. See LICENSE file in root directory.
// Copyright(c) 2016 Intel Corporation. All Rights Reserved.

'use strict';

let os = require('os');
let jpeg = require('jpeg-turbo');
let express = require('express');
let app = express();
let server = require('http').createServer(app);
let WsServer = require('ws').Server;
let slamModule = require('node-slam');

const mapResolution = 0.025;

let slam;
slamModule.createInstance().then((instance) => {
  startServer();
  slam = instance;

  let slamOptions = {
    occupancyMapResolution: mapResolution,
  };
  return slam.setInstanceOptions(slamOptions);
}).then(() => {
  console.log('Starting SLAM...');
  slam.on('tracking', (result) => {
    sendCameraPose(result);
    // FIXME: involking getOccupancyMapUpdate in 'tracking' callback
    // direct will cause crash issue.
    // The crash point is req.Resolve() in AsyncCallback function.
    // The request pointer seems valid, but Resolve member seems inaccessible.
    // Wrap the code block with setTimeout will walk around this issue.
    // We need to fix it, or change the interface format by event to get map
    // update data.
    setTimeout(() => {
      slam.getOccupancyMapUpdate().then((mapData) => {
        sendOccupancyMap(mapData);
      });
    }, 0);
    slam.getTrackingResult().then((result) => {
      updateAndSendFps(result);
      sendFishEyeFrame(result.frameData);
    });
  });
  return slam.start();
}).catch((error) => {
  console.log('error: ' + error);
});

function sendCameraPose(result) {
  if (!connected) {
    return;
  }
  let pose = result.cameraPose;
  let accuracy = result.accuracy;
  let resultToDisplay = {
    type: 'tracking',
    tracking: accuracy,
    pose: [],
  };
  pose.forEach((value) => {
    resultToDisplay.pose.push(value);
  });
  sendData(JSON.stringify(resultToDisplay));
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
    console.log('Stopping...');
    slam.stop().then(() => {
      process.exit();
    }).catch((error) => {
      console.log('error: ' + error);
      process.exit();
    });
  } else {
    process.exit();
  }
}

function sendOccupancyMap(mapData) {
  if (!connected)
    return;

  if (mapData.tileCount <= 0)
    return;

  // struct MsgMapUpdate
  // {
  //   MsgType type;   ///< b[0]
  //   uint8_t _pad[1];///<  [1]
  //   uint16_t scale; ///< b[2-3] in millimeters
  //   int data[0];    ///< b[4]
  // };

  console.log(`Tiles Updated: ${mapData.tileCount}`);

  const msgHeaderLength = 4;
  const MSG_MAPUDPATE = 1;
  let msg = new ArrayBuffer(msgHeaderLength + mapData.data.byteLength);
  let uint8View = new Uint8Array(msg);
  let uint16View = new Uint16Array(msg);
  let int32View = new Int32Array(msg);
  uint8View[0] = MSG_MAPUDPATE;  // type
  uint8View[1] = 0;  // padding
  uint16View[1] = mapResolution * 1000;
  int32View.set(mapData.data, msgHeaderLength / int32View.BYTES_PER_ELEMENT);

  sendData(msg);
}

class FpsCounter {
  constructor(type, maxLimit) {
    this.type = type;
    this.maxLimit = maxLimit;
    this.frameCount = 0;
    this.previousTime = 0;
    this.currentFPS = 0;
    this.timeSumInterval = 0;
  }

  addSamples(count) {
    if(count > 0) {
      let currrenTime = Date.now();
      if(this.previousTime === 0) {
        this.previousTime = currrenTime;
        return;
      }

      this.timeSumInterval += (currrenTime - this.previousTime);
      this.previousTime = currrenTime;
      this.frameCount += count;

      if(this.frameCount >= this.maxLimit) {
        this.currentFPS = (this.frameCount * 1000) / this.timeSumInterval;
        this.timeSumInterval = 0;
        this.frameCount = 0;
      }
    }
  }

  get fps() {
    return this.currentFPS;
  }
}

let fisheyeFps = new FpsCounter('fisheye', 30);
let depthFps = new FpsCounter('depth', 30);
let accelFps = new FpsCounter('accelerometer', 125);
let gyroFps = new FpsCounter('gyroscope', 100);

function updateAndSendFps(result) {
  if (result.frameData.fisheye)
    fisheyeFps.addSamples(1);
  if (result.frameData.depth)
    depthFps.addSamples(1);
  if (result.frameData.accel)
    accelFps.addSamples(result.frameData.accel.frameNumber);
  if (result.frameData.gyro)
    gyroFps.addSamples(result.frameData.gyro.frameNumber);
  if (!connected)
    return;
  let fpsMsg = {
    type: 'fps',
    fps: {
      type: 'tracking',
      fisheye: fisheyeFps.fps,
      depth: depthFps.fps,
      accelerometer: accelFps.fps,
      gyroscope: gyroFps.fps,
    },
  };
  sendData(JSON.stringify(fpsMsg));
}

function sendFishEyeFrame(frame) {
  if (!connected) {
    return;
  }

  // struct MsgImage
  // {
  //    MsgType  type;    // b[0]
  //    uint8_t  format;  // b[1]
  //    uint16_t width;   // b[2-3]
  //    uint16_t height;  // b[4-5]
  //    uint8_t  _pad[2]; //  [6-7]
  //    uint64_t nanos;   // b[8-15]
  //    uint8_t  data[0]; // b[16-];
  // };

  const msgHeaderLength = 16;
  const MSG_FISHEYE = 2;
  const FRAME_FORMAT_JPEG = 1;
  let width = frame.fisheye.width;
  let height = frame.fisheye.height;
  let rawData = frame.fisheye.data;
  let imageBuffer = encodeToJPEG(rawData, width, height);
  let imageBufferLength = imageBuffer.byteLength;

  let msgImage = new ArrayBuffer(msgHeaderLength + imageBufferLength);
  let uint8View = new Uint8Array(msgImage);
  let uint16View = new Uint16Array(msgImage, 0, msgHeaderLength);

  uint8View[0] = MSG_FISHEYE;
  uint8View[1] = FRAME_FORMAT_JPEG;
  uint16View[1] = width;
  uint16View[2] = height;
  uint16View[3] = 0;
  uint8View.set(imageBuffer, msgHeaderLength);

  sendData(msgImage);
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
    format: jpeg.FORMAT_GRAY,
    width: width,
    height: height,
    quality: 80,
    subsampling: jpeg.SAMP_GRAY,
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

function startServer() {
  // Share the ui-browser code from cpp sample
  app.use(express.static('./ui-browser'));
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
      if (msg instanceof Buffer) {
        if (msg[0] === 0xff && msg[1] === 0x02) {
          // TODO(huningxin): handle ack message.
        }
      } else {
        let msgObject = JSON.parse(msg);
        if (msgObject.type === 'control') {
          if (msgObject.command === 'reset') {
            if (slam) {
              slam.restartTracking().then(() => {
                let msgObject = {
                  type: 'event',
                  event: 'on_reset_completed',
                };
                client.send(JSON.stringify(msgObject));
                console.log('Restart--------------------------------');
              }).catch((error) => {
                console.log('error: ' + error);
              });
            }
          }
        }
      }
    });
  });
}
