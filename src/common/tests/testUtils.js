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
      size: ['object', true, 'Size2D'],
      topLeft: ['object', true, 'Point2D'],
      topRight: ['object', true, 'Point2D'],
      bottomLeft: ['object', true, 'Point2D'],
      bottomRight: ['object', true, 'Point2D'],
      center: ['object', true, 'Point2D'],
      topCenter: ['object', true, 'Point2D'],
      bottomCenter: ['object', true, 'Point2D'],
      leftCenter: ['object', true, 'Point2D'],
      rightCenter: ['object', true, 'Point2D'],
      top: ['number', true],
      left: ['number', true],
      bottom: ['number', true],
      right: ['number', true],
      horzRange: ['object', true, 'Range'],
      vertRange: ['object', true, 'Range'],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
      if(attrs[k][2] === 'Size2D') {
         assert.equal(isSize2DObject(instance[k]), true);
      }else if(attrs[k][2] === 'Point2D') {
         assert.equal(isPoint2DObject(instance[k]), true);
      }else if(attrs[k][2] === 'Range') {
         assert.equal(isRangeObject(instance[k]), true);
      }
    };
    [
      'equal', 'isEmpty', 'isValid',
      'adjust', 'expand', 'shrink', 'move',
      'moveTopTo', 'moveLeftTo', 'moveRightTo', 'moveBottomTo', 'moveCenterTo',
      'moveTopLeftTo', 'moveTopRightTo', 'moveBottomRightTo', 'moveBottomLeftTo',
      'moveLeftCenterTo', 'moveTopCenterTo', 'moveRightCenterTo', 'moveBottomCenterTo',
      'normalize', 'setCoords', 'getCoords', 'onEdge',
      'contains', 'containsCoord', 'properlyContains',
      'properlyContainsCoord', 'intersects', 'intersectsCoord',
      'unite', 'uniteCoord', 'split', 'splitX', 'splitY',
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

function isRangeObject(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      begin: ['number', true],
      end: ['number', true],
      min: ['number', true],
      max: ['number', true],
      length: ['number', false],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    [
      'equal', 'isEmpty', 'isValid', 'contains', 'properlyContains',
    ].forEach((methodName) => {
      assert.equal(typeof instance[methodName], 'function');
    });
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isSize2DObject(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      width: ['number', true],
      height: ['number', true],
      cx: ['number', true],
      cy: ['number', true],
    };
    // eslint-disable-next-line
    for(let k in attrs) {
      assert(instance.hasOwnProperty(k));
      assert.equal(typeof instance[k], attrs[k][0]);
      assert.equal(Object.getOwnPropertyDescriptor(instance, k).writable, attrs[k][1]);
    };
    [
      'equal', 'isEmpty', 'isValid',
    ].forEach((methodName) => {
      assert.equal(typeof instance[methodName], 'function');
    });
    return true;
  }catch(e) {
    console.log(e);
    return e;
  }
}

function isPoint2DObject(instance) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      x: ['number', true],
      y: ['number', true],
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

function isFrameData(instance, module) {
  try {
    let attrs = {
      // attributeName: [type, writable]
      // color is undefined for SLAM
      // color: ['Image', false],
      fisheye: ['Image', false],
      depth: ['Image', false],
      gyro: ['MotionSample', false],
      accel: ['MotionSample', false],
    };
    if(module === 'or') {
      attrs = {
      // attributeName: [type, writable]
      // fisheys, gyro accel is undefined for SLAM
      // fisheye: ['Image', false],
      // gyro: ['MotionSample', false],
      // accel: ['MotionSample', false],
      color: ['Image', false],
      depth: ['Image', false],
    };
    
    }
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

