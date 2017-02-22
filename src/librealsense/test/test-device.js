// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const assert = require('assert');
const addon = require('..');

describe('Test Device interfaces', function() {
  let context = new addon.Context;
  let device = null;
  let mode = {
    width: 640,
    height: 480,
    format: 'rgb8',
    framerate: 30,
  };

  before(function(done) {
    context.getDevice(0).then((availableDevice) => {
      device = availableDevice;
      context.device = device;
      done();
    });
  });

  it('Get the device name at index 0', function() {
    return new Promise((resolve, reject) => {
      device.getName().then((name) => {
        assert.equal(typeof name, 'string');
        console.log('The name is ' + name);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get the depth scale', function() {
    return new Promise((resolve, reject) => {
      device.getDepthScale().then((scale) => {
        console.log('The depth scale is ' + scale);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get firmware version', function() {
    return new Promise((resolve, reject) => {
      device.getFirmwareVersion().then((firmware) => {
        console.log('The firmware version is ' + firmware);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get serial', function() {
    return new Promise((resolve, reject) => {
      device.getSerial().then((serial) => {
        console.log('The serial is ' + serial);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get streaming status', function() {
    return new Promise((resolve, reject) => {
      device.isStreaming().then((isStreaming) => {
        console.log('The device streaming status ' + isStreaming);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Option support status', function() {
    return new Promise((resolve, reject) => {
      device.supportsOption('r200_depth_units').then((isSupport) => {
        console.log('The device r200_depth_units option status ' + isSupport);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Enable stream', function() {
    return new Promise((resolve, reject) => {
      let mode = {
        width: 640,
        height: 480,
        format: 'rgb8',
        framerate: 30,
      };

      device.enableStream('color', mode).then(() => {
        device.isStreamEnabled('color').then((enable) => {
          if (enable)
            console.log('The color stream has been enabled.');
          resolve();
        });
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get stream mode count', function() {
    return new Promise((resolve, reject) => {
      device.getStreamModeCount('color').then((count) => {
        console.log('The color stream count is ' + count);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get stream enable status', function() {
    return new Promise((resolve, reject) => {
      device.isStreamEnabled('color').then((enable) => {
        console.log('The color stream enable status ' + enable);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });


  it('Get stream width', function() {
    return new Promise((resolve, reject) => {
      device.getStreamWidth('color').then((width) => {
        console.log('The stream width is ' + width);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get stream height', function() {
    return new Promise((resolve, reject) => {
      device.getStreamHeight('color').then((height) => {
        console.log('The stream height is ' + height);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Get stream mode', function() {
    return new Promise((resolve, reject) => {
      device.getStreamMode('color', 0).then((mode) => {
        console.log('width is ' + mode.width);
        console.log('height is ' + mode.height);
        console.log('format is ' + mode.format);
        console.log('framerate is ' + mode.framerate);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Start/Stop the device, and frame timestamp', function() {
    return new Promise((resolve, reject) => {
      device.enableStream('color', mode).then((enable) => {
        device.start().then(() => {
          console.log('Device starts.');
          device.getFrameTimeStamp('color').then((timestamp) => {
            console.log('The stream timestamp is ' + timestamp);
              device.stop().then(() => {
              console.log('Device stopped.');
              resolve();
            });
          });
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
});
