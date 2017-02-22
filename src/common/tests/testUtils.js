 // Copyright (c) 2016 Intel Corporation. All rights reserved.
 // Use of this source code is governed by a MIT-style license that can be
 // found in the LICENSE file.

'use strict';
const assert = require('assert');
function isRect2DObject(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      x: ['number', true],
      y: ['number', true],
      width: ['number', true],
      height: ['number', true],
      diagonalLength: ['number', false],
      size: ['object', true],
      topLeft: ['object', true],
      topRight: ['object', true],
      bottomLeft: ['object', true],
      bottomRight: ['object', true],
      center: ['object', true],
      topCenter: ['object', true],
      bottomCenter: ['object', true],
      leftCenter: ['object', true],
      rightCenter: ['object', true],
      top: ['number', true],
      left: ['number', true],
      bottom: ['number', true],
      right: ['number', true],
      horzRange: ['object', true],
      vertRange: ['object', true],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    [
      'equal', 'isEmpty', 'isValid',
      'adjust', 'expand', 'shrink', 'move',
      'moveTopTo', 'moveLeftTo', 'moveRightTo', 'moveBottomTo', 'moveCenterTo',
      'moveTopLeftTo', 'moveTopRightTo', 'moveBottomRightTo', 'moveBottomLeftTo',
      'moveLeftCenterTo', 'moveTopCenterTo', 'moveRightCenterTo', 'moveBottomCenterTo',
      'normalize', 'setCoords', 'getCoords', 'onEdge',
      'contains', 'properlyContains', 'properlyContainsCoord', 'intersects',
      'unite', 'split', 'splitX', 'splitY',
      'united', 'unitedCoord', 'intersected', 'intersectedCoord',
    ].forEach((methodName) => {
      assert.equal(typeof instance[methodName], 'function');
    });
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isPoint3DObject(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      x: ['number', true],
      y: ['number', true],
      z: ['number', true],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    [
      'equal', 'setCoords', 'getCoords',
    ].forEach((methodName) => {
      assert.equal(typeof instance[methodName], 'function');
    });
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isImage(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      width: ['number', false],
      height: ['number', false],
      format: ['string', false],
      timeStamp: ['number', false],
      frameNumber: ['number', false],
      data: ['object', false],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isMotionSample(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      timeStamp: ['number', false],
      frameNumber: ['number', false],
      data: ['object', false],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isFrameData(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      fisheye: ['Image', false],
      // color is undefined for SLAM
      // color: ['Image', false],
      depth: ['Image', false],
      gyro: ['MotionSample', false],
      accel: ['MotionSample', false],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], 'object');
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
      if(attrs[k][0] === 'Image') {
        assert.equal(isImage(instance[k]), true);
      }else if(attrs[k][0] === 'MotionSample') {
        assert.equal(isMotionSample(instance[k]), true);
      }
    };
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}
module.exports = {
  isRect2DObject: isRect2DObject,
  isPoint3DObject: isPoint3DObject,
  isFrameData: isFrameData,
};

