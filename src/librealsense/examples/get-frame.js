// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('..');
let fs = require('fs');
let jpeg = require('jpeg-turbo');

let context = new addon.Context;

context.getDevice(0).then((device) => {
  let mode = {
    width: 640,
    height: 480,
    format: 'rgb',
    framerate: 30,
  };

  let frameCount = 0;

  console.log('One frame will be saved as a JPEG file every 10 seconds.');

  device.on('frameready', function(frame) {
    if (frameCount % 300 === 0) {
      let imageBuffer;
      imageBuffer = encodeToJPEG(frame.data, 640, 480);

      let fileName = 'frame_' + frameCount + '.jpg';
      fs.writeFileSync(fileName, imageBuffer);
      console.log('Frame has been saved as ' + fileName);
    }

    frameCount ++;
  });

  device.enableStream('color', mode).then(() => {
    device.start().catch((e) => {
      console.log(e);
    });
  });

  device.context = context;
}).catch((e) => {
  console.log(e);
});

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
