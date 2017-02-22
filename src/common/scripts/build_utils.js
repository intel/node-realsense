// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

function _runNodeGyp() {
  return new Promise((resolve, reject) => {
    const buildProc = require('child_process').spawn(
        'node-gyp',
        ['rebuild', '-j', require('os').cpus().length],
        {cwd: '.', stdio: 'inherit'});

    buildProc.on('close', (code) => {
      if (code !== 0) process.exit(code);

      resolve(code);
    });

    buildProc.stdout.on('data', (data) => {
      console.log(`${data}`);
    });
    buildProc.stderr.on('data', (data) => {
      console.log(`${data}`);
    });
  });
}

module.exports = {
  runNodeGyp: _runNodeGyp,
};
