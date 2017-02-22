 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';
const pt = require('bindings')('realsense_person_tracking');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(pt.PersonTracker, EventEmitter);

describe('Person Tracking Test Suite - CameraOptions - Full', function() {
  it('setCameraOptions() can work', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60000);
    return new Promise(function(resolve, reject) {
      let tracker = null;
      pt.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(function() {
        return tracker.setCameraOptions({
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
        });
      }).then(function() {
        return tracker.setCameraOptions();
      }).then(function() {
        return tracker.setCameraOptions({});
      }).then(function() {
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Wrong options should be rejected by setCameraOptions()', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60000);
    return new Promise(function(resolve, reject) {
      let tracker = null;
      pt.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.setCameraOptions({
          color: {
            width: 3840,
            height: 2160,
            frameRate: 120,
            isEnabled: true,
          },
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(function() {
        reject('Should not accept 4x FullHD resolution options for color channel');
      }).catch(function(e) {
        resolve();
      });
    });
  });

  it('setCameraOptions() should not overwrite member which is not in dictionary', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60000);
    let previousOpt;
    return new Promise(function(resolve, reject) {
      let tracker = null;
      pt.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.getCameraOptions();
      }).then(function(options) {
        previousOpt = options;
        return tracker.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(previousOpt.color.width, options.color.width);
        assert.equal(previousOpt.color.height, options.color.height);
        assert.equal(previousOpt.color.frameRate, options.color.frameRate);
        assert.equal(previousOpt.color.isEnabled, options.color.isEnabled);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Sent validate options to setCameraOptions() and get back correctly', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60000);
    const myOptions = {
      color: {
        width: 1920,
        height: 1080,
        frameRate: 30,
        isEnabled: true,
      },
      depth: {
        width: 628,
        height: 468,
        frameRate: 30,
        isEnabled: true,
      },
    };
    let tracker;
    return new Promise(function(resolve, reject) {
      pt.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.notEqual(myOptions.color.width, options.color.width);
        assert.notEqual(myOptions.color.height, options.color.height);
        assert.notEqual(myOptions.depth.width, options.depth.width);
        assert.notEqual(myOptions.depth.height, options.depth.height);
        return tracker.setCameraOptions(myOptions);
      }).then(function() {
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(myOptions.color.width, options.color.width);
        assert.equal(myOptions.color.height, options.color.height);
        assert.equal(myOptions.color.frameRate, options.color.frameRate);
        assert.equal(myOptions.color.isEnabled, options.color.isEnabled);
        assert.equal(myOptions.depth.width, options.depth.width);
        assert.equal(myOptions.depth.height, options.depth.height);
        assert.equal(myOptions.depth.frameRate, options.depth.frameRate);
        assert.equal(myOptions.depth.isEnabled, options.depth.isEnabled);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });

  it('Sent invalidate options to setCameraOptions() and shoule ignore', function() {
    // eslint-disable-next-line no-invalid-this
    this.timeout(60000);
    const myOptions = {
      color: {
        width: 320,
        height: 240,
        frameRate: 30,
        isEnabled: true,
        dummy: 'dummy',
      },
      depth: {
        width: 320,
        height: 240,
        frameRate: 30,
        isEnabled: true,
        dummy: 'dummy',
      },
    };
    let tracker;
    return new Promise(function(resolve, reject) {
      pt.createPersonTracker().then(function(inst) {
        tracker = inst;
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.notEqual(myOptions.color.width, options.color.width);
        assert.notEqual(myOptions.color.height, options.color.height);
        assert.notEqual(myOptions.depth.width, options.depth.width);
        assert.notEqual(myOptions.depth.height, options.depth.height);
        assert.equal(options.color.dummy, undefined);
        assert.equal(options.depth.dummy, undefined);
        return tracker.setCameraOptions(myOptions);
      }).then(function() {
        return tracker.getCameraOptions();
      }).then(function(options) {
        assert.equal(myOptions.color.width, options.color.width);
        assert.equal(myOptions.color.height, options.color.height);
        assert.equal(myOptions.color.frameRate, options.color.frameRate);
        assert.equal(myOptions.color.isEnabled, options.color.isEnabled);
        assert.equal(myOptions.depth.width, options.depth.width);
        assert.equal(myOptions.depth.height, options.depth.height);
        assert.equal(myOptions.depth.frameRate, options.depth.frameRate);
        assert.equal(myOptions.depth.isEnabled, options.depth.isEnabled);
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
});

function testSetPositive(data) {
  describe('Person Tracking Test Suite - CameraOptions - Positive', function() {
    it('[set] Validate valid different camera options - positive -  color:'
      + data.color.width + 'x' + data.color.height + ' - depth:'
      + data.depth.width + 'x' + data.depth.height, function() {
    // eslint-disable-next-line no-invalid-this
      this.timeout(20000);
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      let tracker = null;
      return new Promise(function(resolve, reject) {
        pt.createPersonTracker().then(function(inst) {
          tracker = inst;
          return tracker.setCameraOptions(cameraOptions);
        }).then(function() {
          return tracker.getCameraOptions();
        }).then(function(options) {
          assert.equal(cameraOptions.color.width, options.color.width);
          assert.equal(cameraOptions.color.height, options.color.height);
          assert.equal(cameraOptions.color.frameRate, options.color.frameRate);
          assert.equal(cameraOptions.color.isEnabled, options.color.isEnabled);
          assert.equal(cameraOptions.depth.width, options.depth.width);
          assert.equal(cameraOptions.depth.height, options.depth.height);
          assert.equal(cameraOptions.depth.frameRate, options.depth.frameRate);
          assert.equal(cameraOptions.depth.isEnabled, options.depth.isEnabled);
          resolve();
        }).catch(function(e) {
          reject(e);
        });
      });
    });
  });
}

function testSetNegative(data) {
  describe('Person Tracking Test Suite - CameraOptions - Negative', function() {
    it('[set] Validate invalid different camera options - positive - color:'
      + data.color.width + 'x' + data.color.height + ' - color frameRate:'
      + data.color.frameRate + ' - depth:'
      + data.depth.width + 'x' + data.depth.height + ' - depth frameRate:'
      + data.depth.frameRate, function() {
    // eslint-disable-next-line no-invalid-this
      this.timeout(20000);
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      let tracker = null;
      return new Promise(function(resolve, reject) {
        pt.createPersonTracker().then(function(inst) {
          return tracker.setCameraOptions(cameraOptions);
        }).then(function() {
          return tracker.getCameraOptions();
        }).then(function(options) {
          reject('should fail but promise got resolved');
        }).catch(function(e) {
          resolve();
        });
      });
    });
  });
}
function testCreatePositive(data) {
  describe('Person Tracking Test Suite - CameraOptions - Positive', function() {
    it('[create] Validate valid different camera options - positive -  color:'
      + data.color.width + 'x' + data.color.height + ' - depth:'
      + data.depth.width + 'x' + data.depth.height, function() {
    // eslint-disable-next-line no-invalid-this
      this.timeout(20000);
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      let tracker = null;
      return new Promise(function(resolve, reject) {
        pt.createPersonTracker(cameraOptions).then(function(inst) {
          tracker = inst;
          return tracker.getCameraOptions();
        }).then(function(options) {
          assert.equal(cameraOptions.color.width, options.color.width);
          assert.equal(cameraOptions.color.height, options.color.height);
          assert.equal(cameraOptions.color.frameRate, options.color.frameRate);
          assert.equal(cameraOptions.color.isEnabled, options.color.isEnabled);
          assert.equal(cameraOptions.depth.width, options.depth.width);
          assert.equal(cameraOptions.depth.height, options.depth.height);
          assert.equal(cameraOptions.depth.frameRate, options.depth.frameRate);
          assert.equal(cameraOptions.depth.isEnabled, options.depth.isEnabled);
          resolve();
        }).catch(function(e) {
          reject(e);
        });
      });
    });
  });
}

function testCreateNegative(data) {
  describe('Person Tracking Test Suite - CameraOptions - Negative', function() {
    it('[create] Validate invalid different camera options - positive - color:'
      + data.color.width + 'x' + data.color.height + ' - color frameRate:'
      + data.color.frameRate + ' - depth:'
      + data.depth.width + 'x' + data.depth.height + ' - depth frameRate:'
      + data.depth.frameRate, function() {
    // eslint-disable-next-line no-invalid-this
      this.timeout(20000);
      const cameraOptions = {
        color: {
          width: data.color.width,
          height: data.color.height,
          frameRate: data.color.frameRate,
          isEnabled: true,
        },
        depth: {
          width: data.depth.width,
          height: data.depth.height,
          frameRate: data.depth.frameRate,
          isEnabled: true,
        },
      };
      let tracker = null;
      return new Promise(function(resolve, reject) {
        pt.createPersonTracker(cameraOptions).then(function(inst) {
          return tracker.getCameraOptions();
        }).then(function(options) {
          reject('should fail but promise got resolved');
        }).catch(function(e) {
          resolve();
        });
      });
    });
  });
}

let arrPositive = [
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 320, height: 240, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
  {
    color: {width: 320, height: 240, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 640, height: 480, frameRate: 30},
  },
  {
    color: {width: 640, height: 480, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
];

let arrNegative = [
  {
    color: {width: 30, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 300, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 700, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 123, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 1},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 1},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 50},
    depth: {width: 628, height: 468, frameRate: 30},
  },
  {
    color: {width: 1920, height: 1080, frameRate: 30},
    depth: {width: 628, height: 468, frameRate: 50},
  },
  {
    color: {width: '640', height: 480, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
  {
    color: {width: 640, frameRate: 30},
    depth: {width: 320, height: 240, frameRate: 30},
  },
];

// eslint-disable-next-line guard-for-in
for(let i in arrPositive) {
  testSetPositive(arrPositive[i]);
  testCreatePositive(arrPositive[i]);
}

// eslint-disable-next-line guard-for-in
for(let j in arrNegative) {
  testSetNegative(arrNegative[j]);
  testCreateNegative(arrNegative[j]);
}
