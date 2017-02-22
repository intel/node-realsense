// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

const g = require('widl-nan').generator;

Promise.all(process.argv.slice(3).map(function(val, index, array) {
  return g.addFile(val);
})).then(function() {
  g.compile();
  g.writeToDir(process.argv[2]);
}).catch((e) => {
  console.log(e);
});
