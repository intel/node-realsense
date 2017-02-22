// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const exec = require('child_process').exec;
const path = require('path');

function _tryCleanup(workingDir) {
  return new Promise((resolve, reject) => {
    let cmd = 'rm -rf ' + path.join(workingDir, 'node_modules');
    exec(cmd, function(err, stdout, stderr) {
      if (err) {
        reject(err);
      } else {
        resolve();
      }
    });
  });
}

function _tryInstallFromPath(workingDir, srcDir, spawnNpmInstall) {
  return new Promise((resolve, reject) => {
    spawnNpmInstall([srcDir], {cwd: workingDir}, function(err) {
      if (err) {
        reject(err);
      } else {
        resolve('installed');
      }
    });
  });
}

function _installFromPath(tmp, spawnNpmInstall) {
  return new Promise((resolve, reject) => {
    tmp.dir(function(err, tmpdir, cleanupCallback) {
      if (err) {
        reject(err);
      }

      const src = process.cwd();
      // TODO: This case fake succeed because of output dirs (build, gen, node_modules)
      // under src are not cleanup. If we do so, this case will fail.
      _tryInstallFromPath(tmpdir, src, spawnNpmInstall).then(function() {
        return _tryCleanup(tmpdir);
      }).then(() => {
        cleanupCallback();
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
}

function _makeTarball() {
  return new Promise((resolve, reject) => {
    let cmd = 'rm -rf *.gz && npm run-script maketarball';
    exec(cmd, function(err, stdout, stderr) {
      if (err) {
        reject(err);
      } else {
        resolve();
      }
    });
  });
}

function _getTarballName(srcDir) {
  let config = path.join(srcDir, 'package.json');
  return path.join(srcDir, require(config).name + '-' + require(config).version + '.tar.gz');
}

function _tryInstallFromTarball(workingDir, srcDir, spawnNpmInstall) {
  return new Promise((resolve, reject) => {
    let tarball = _getTarballName(srcDir);
    spawnNpmInstall([tarball], {cwd: workingDir}, function(err) {
      if (err) {
        reject(err);
      } else {
        resolve('installed');
      }
    });
  });
}

function _installFromTarball(tmp, spawnNpmInstall) {
  return new Promise((resolve, reject) => {
    tmp.dir(function(err, tmpdir, cleanupCallback) {
      if (err) {
        reject(err);
      }

      const src = process.cwd();
      _tryInstallFromTarball(tmpdir, src, spawnNpmInstall).then(function() {
        return _tryCleanup(tmpdir);
      }).then(() => {
        cleanupCallback();
        resolve();
      }).catch(function(e) {
        reject(e);
      });
    });
  });
}

module.exports = {
  installFromPath: _installFromPath,
  makeTarball: _makeTarball,
  installFromTarball: _installFromTarball,
};
