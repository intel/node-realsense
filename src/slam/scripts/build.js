// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

const spawn = require('child_process').spawn;
const w = require('widl-nan');
const g = w.generator;

function build() {
  return new Promise((resolve, reject) => {
    const buildProc = spawn('node-gyp', ['rebuild'], {cwd: '.'});

    buildProc.on('exit', function(code) {
      if (code !== 0) {
        process.exit(code);
      }

      resolve(code);
    });
  });
}

Promise.all([
    g.addFile('idl/common/realsense-camera.widl'),
    g.addFile('idl/common/geometry.widl'),
    g.addFile('idl/slam/slam.widl'),
  ]).then(function() {
    g.compile();
    g.writeToDir('gen');
    build().then(function(code) {
      console.log('Success');
    });
  }).catch((e) => {
    console.log(e);
  });
