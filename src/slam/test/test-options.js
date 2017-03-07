 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';

const slamAddon = require('bindings')('realsense_slam');
const EventEmitter = require('events').EventEmitter;
const assert = require('assert');

function inherits(target, source) {
  // eslint-disable-next-line guard-for-in
  for (let k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}
inherits(slamAddon.Instance, EventEmitter);

describe('Slam Test Suite -Run', function() {
  // eslint-disable-next-line max-len
  it('Validate options sent to setInstanceOptions() by getting them back from getInstanceOptions()', function() {
    let InstanceOptions = {
      enableOccupancyMapBuilding: true,
      occupancyMapResolution: 0.99,
      occupancyMapHeightOfInterest: {min: -0.5, max: 1},
      enableRelocalizationMapping: true,
      enableForceRelocalizationPose: true,
    };
    return slamAddon.createInstance()
    .then(function(Instance) {
      return Instance.setInstanceOptions(InstanceOptions)
      .then(function() {
        return Instance.getInstanceOptions();
      });
    })
    .then(function(options) {
      assert.equal(options.occupancyMapResolution.toFixed(2),
        InstanceOptions.occupancyMapResolution);
      assert.equal(options.enableOccupancyMapBuilding,
        InstanceOptions.enableOccupancyMapBuilding);
      assert.equal(options.occupancyMapHeightOfInterest.max,
        InstanceOptions.occupancyMapHeightOfInterest.max);
      assert.equal(options.occupancyMapHeightOfInterest.min,
        InstanceOptions.occupancyMapHeightOfInterest.min);
      assert.equal(options.enableRelocalizationMapping,
        InstanceOptions.enableRelocalizationMapping);
      assert.equal(options.enableForceRelocalizationPose,
        InstanceOptions.enableForceRelocalizationPose);
    })
    .catch(function(err) {
      console.log(err);
      assert.ok(false);
    });
  });

  it('getCameraOptions() should return meaningful data by default', function() {
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((Instance) => {
        return Instance.getCameraOptions();
      }).then((CameraOptions) => {
        assert.equal(typeof CameraOptions, 'object');
        assert.equal(typeof CameraOptions.color, 'object');
        assert.equal(typeof CameraOptions.color.width, 'number');
        assert.equal(typeof CameraOptions.color.height, 'number');
        assert.equal(typeof CameraOptions.color.isEnabled, 'boolean');
        assert.equal(typeof CameraOptions.color.frameRate, 'number');
        assert.equal(typeof CameraOptions.depth, 'object');
        assert.equal(typeof CameraOptions.depth.width, 'number');
        assert.equal(typeof CameraOptions.depth.height, 'number');
        assert.equal(typeof CameraOptions.depth.isEnabled, 'boolean');
        assert.equal(typeof CameraOptions.depth.frameRate, 'number');
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('setCameraOptions() can work', function() {
    let instance;
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((inst) => {
        instance = inst;
        return instance.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        return instance.setCameraOptions({
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
      }).then(()=> {
        return instance.setCameraOptions();
      }).then(()=> {
        return instance.setCameraOptions({});
      }).then(()=> {
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Wrong options should be rejected by setCameraOptions()', function() {
    let Instance;
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((inst) => {
      Instance = inst;
      return Instance.setCameraOptions({
        fisheye: {
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
      }).then(() => {
        reject('Should not accept 4x FullHD resolution options for color channel');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('setCameraOptions() should not overwrite member which is not in dictionary', function() {
    let previousOptions;
    let instance;
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((inst) => {
        instance = inst;
        return inst.getCameraOptions();
      }).then((options)=> {
        previousOptions = options;
        return instance.setCameraOptions({
          depth: {
            width: 320,
            height: 240,
            frameRate: 30,
            isEnabled: true,
          },
        });
      }).then(()=> {
        return instance.getCameraOptions();
      }).then((options)=> {
        assert.equal(previousOptions.fisheye.width, options.fisheye.width);
        assert.equal(previousOptions.fisheye.height, options.fisheye.height);
        assert.equal(previousOptions.fisheye.frameRate, options.fisheye.frameRate);
        assert.equal(previousOptions.fisheye.isEnabled, options.fisheye.isEnabled);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  // SLAM module only supports 640*480 resolution for fisheye and 320*240 for depth image.
  it.skip('Sent validate options to setCameraOptions() and get back correctly', function() {
    const myOptions = {
      fisheye: {
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
    let instance;
    return new Promise((resolve, reject) => {
      slamAddon.createInstance().then((inst) => {
        instance = inst;
        return inst.getCameraOptions();
      }).then((options)=> {
        assert.notEqual(myOptions.fisheye.width, options.fisheye.width);
        assert.notEqual(myOptions.fisheye.height, options.fisheye.height);
        assert.notEqual(myOptions.depth.width, options.depth.width);
        assert.notEqual(myOptions.depth.height, options.depth.height);
        return instance.setCameraOptions(myOptions);
      }).then(()=> {
        return instance.getCameraOptions();
      }).then((options)=> {
        assert.equal(myOptions.fisheye.width, options.fisheye.width);
        assert.equal(myOptions.fisheye.height, options.fisheye.height);
        assert.equal(myOptions.fisheye.frameRate, options.fisheye.frameRate);
        assert.equal(myOptions.fisheye.isEnabled, options.fisheye.isEnabled);
        assert.equal(myOptions.depth.width, options.depth.width);
        assert.equal(myOptions.depth.height, options.depth.height);
        assert.equal(myOptions.depth.frameRate, options.depth.frameRate);
        assert.equal(myOptions.depth.isEnabled, options.depth.isEnabled);
        resolve();
      }).catch((e) => {
        reject(e);
      });
    });
  });

  it('Sent validate options to createInstance() and get back correctly', function() {
    let InstanceOptions= {
      enableOccupancyMapBuilding: true,
      occupancyMapResolution: 0.99,
      occupancyMapHeightOfInterest: {min: -0.5, max: 1},
      enableRelocalizationMapping: true,
      enableForceRelocalizationPose: true,
    };
    let instance;
    return slamAddon.createInstance()
    .then(function(inst) {
      instance = inst;
      // Test optional argument.
      return instance.setInstanceOptions();
    })
    .then(function() {
      return instance.setInstanceOptions(InstanceOptions);
    })
    .then(function() {
      return instance.getInstanceOptions();
    }).then(function(options) {
      assert.equal(options.enableOccupancyMapBuilding,
        InstanceOptions.enableOccupancyMapBuilding);
      assert.equal(options.occupancyMapHeightOfInterest.min,
        InstanceOptions.occupancyMapHeightOfInterest.min);
      assert.equal(options.occupancyMapHeightOfInterest.max,
        InstanceOptions.occupancyMapHeightOfInterest.max);
      assert.equal(options.enableRelocalizationMapping,
        InstanceOptions.enableRelocalizationMapping);
      assert.equal(options.enableForceRelocalizationPose,
        InstanceOptions.enableForceRelocalizationPose);
      assert.equal(options.occupancyMapResolution.toFixed(2),
        InstanceOptions.occupancyMapResolution);
    }).catch(function(err) {
      console.log(err);
      assert.ok(false);
    });
  });
});
