// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const addon = require('..');

addon.createObjectRecognizer().then((objectRecognizer) => {
  objectRecognizer.setCameraOptions({
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
  }).then(() => {
    return objectRecognizer.start();
  }).then(() => {
    console.log("So far so good");
    return objectRecognizer.getRecognizableObjectNames();
  }).then((names) => {
    console.log(names);
    console.log(objectRecognizer);
  }).catch((e) => {
    console.log(e);
  });
});
