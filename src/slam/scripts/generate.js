// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

const w = require('widl-nan');
const g = w.generator;

Promise.all([
    g.addFile('idl/common/realsense-camera.widl'),
    g.addFile('idl/common/geometry.widl'),
    g.addFile('idl/slam/slam.widl'),
  ]).then(function() {
    g.compile();
    g.writeToDir('gen');
  }).catch((e) => {
    console.log(e);
  });
