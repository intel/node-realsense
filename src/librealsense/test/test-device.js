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
        assert.equal(typeof scale, 'number');
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
        assert.equal(typeof firmware, 'string');
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
        assert.equal(typeof serial, 'string');
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
        assert.equal(typeof isStreaming, 'boolean');
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
        assert.equal(typeof isSupport, 'boolean');
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
            assert.equal(typeof enable, 'boolean');
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
        assert.equal(typeof count, 'number');
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
        assert.equal(typeof width, 'number');
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
        assert.equal(typeof height, 'number');
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
        assert.equal(typeof mode, 'object');
        assert.equal(typeof mode.width, 'number');
        assert.equal(typeof mode.height, 'number');
        assert.equal(typeof mode.format, 'string');
        assert.equal(typeof mode.framerate, 'number');
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
      device.enableStream('color', mode).then((voidData) => {
        assert.equal(typeof voidData, 'undefined');
        device.start().then((voidData) => {
          assert.equal(typeof voidData, 'undefined');
          console.log('Device starts.');
          device.getFrameTimeStamp('color').then((timestamp) => {
            assert.equal(typeof timestamp, 'number');
            console.log('The stream timestamp is ' + timestamp);
            device.stop().then(() => {
              assert.equal(typeof voidData, 'undefined');
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
