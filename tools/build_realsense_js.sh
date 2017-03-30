#!/bin/bash
#
# Copyright (c) 2016 Intel Corporation. All rights reserved.
# Use of this source code is governed by a MIT-style license that can be
# found in the LICENSE file.
#

set -e

MOUDLES="
object-recognition
person-tracking
slam
librealsense
"

pushd $(dirname $0) > /dev/null

CONFIG_FILE="./.boto_config.json"
if [ ! -f "${CONFIG_FILE}" ]; then
 echo -e "${CONFIG_FILE} is not avialable. Do Nothing!"
 echo -e "If you need want to build, please make a copy from ${CONFIG_FILE}.template and customize."
 exit 0
fi

# Try to enable SSH w/p password, which is needed for scp/ssh and git pull/push
NOPASS_EXPERCT="./.no_password.expect"
if [ ! -f "${NOPASS_EXPERCT}" ]; then
 echo -e "${NOPASS_EXPERCT} is not avialable. Do Nothing!"
 echo -e "If you need want to build, please make a copy from ${NOPASS_EXPERCT}.template and customize."
 exit 0
fi

[[ -z ${SSH_AGENT_PID} ]] && eval `ssh-agent`
expect ${NOPASS_EXPERCT}
git pull --rebase

# Get archive server config
REMOTE_SERVER=$(node -e "console.log(require('${CONFIG_FILE}').archive_server.host);")
NIGHTLY_DIR=$(node -e "console.log(require('${CONFIG_FILE}').archive_server.nightly_dir);")
OFFICAL_DIR=$(node -e "console.log(require('${CONFIG_FILE}').archive_server.official_dir);")
REMOTE_USERNAME=$(node -e "console.log(require('${CONFIG_FILE}').archive_server.username);")
REMOTE_USERGROUP=$(node -e "console.log(require('${CONFIG_FILE}').archive_server.usergroup);")

# Get general config
BUILD_TYPE=$(node -e "console.log(require('${CONFIG_FILE}').build_type);")
BUMP_VERSION=$(node -e "console.log(require('${CONFIG_FILE}').bump_version);")
UPLOAD_BUILD=$(node -e "console.log(require('${CONFIG_FILE}').upload_build);")
TARBALL_ONLY=$(node -e "console.log(require('${CONFIG_FILE}').tarball_only);")

TODAY=$(date +%Y%m%d)
LOGFILE="node_realsense-$(date +%Y%m%d).log"
GITMESSAGE=""
VERSION=""

# Get the archive directory
if [ "${BUILD_TYPE}" = "official" ]; then
  VERSION=$1
  if [ "${VERSION}" = "" ]; then
    echo -e "Need specify version on official build, usage: $0 <version>"
    exit 1
  fi
  ARCHIVE_DIR=${OFFICAL_DIR}/v${VERSION}
  LOGFILE="node_realsense-${VERSION}-$(date +%Y%m%d).log"
elif [ "${BUILD_TYPE}" = "nightly" ]; then
  ARCHIVE_DIR=${NIGHTLY_DIR}/${TODAY}
else
  echo -e "Unknown build type defined in ${CONFIG_FILE}. Only official and nightly supported."
  exit 1
fi

# Create archive dir
[[ "${UPLOAD_BUILD}" = "true" ]] && ssh ${REMOTE_USERNAME}@${REMOTE_SERVER} "mkdir -p ${ARCHIVE_DIR}"

NPM_CMDS="install;test"

# Execute for each module
function build_module {
  module_name=$1

  echo -e "\n\n---------Building ${module_name}----------"

  cd src/${module_name}

  if [ "${BUMP_VERSION}" = "true" ]; then
    if [ "${BUILD_TYPE}" = "official" ]; then
      npm version ${VERSION}
    else
      # Update patch version on nightly build
      VERSION=$(npm version patch | sed 's/v//g')
    fi
  fi

  # Cleanup environment
  rm -rf node_modules/ build/ *.tar.gz

  if [ "${TARBALL_ONLY}" = "true" ]; then
    npm run-script maketarball >> ${LOGFILE}
    [[ "${UPLOAD_BUILD}" = "true" ]] && scp *.tar.gz ${REMOTE_USERNAME}@${REMOTE_SERVER}:${ARCHIVE_DIR}/
    cd ../..
    return
  fi

  error="0"
  IFS=';'
  for cmd in ${NPM_CMDS}; do
    echo -e "\n\nRun npm $cmd"
    eval "npm $cmd" || error="$cmd"
  done

  if [ "$error" = "0" ]; then
    msg="${module_name}: SUCCEED"

  else
    msg="${module_name}: FAILED on $error"
  fi

  GITMESSAGE="
${GITMESSAGE}
${msg}"
  echo -e "\n\n ${msg}"

  [[ "${UPLOAD_BUILD}" = "true" ]] && scp *.tar.gz ${REMOTE_USERNAME}@${REMOTE_SERVER}:${ARCHIVE_DIR}/
  cd ../..
}

cd ..
HEAD_SHA1=$(git log HEAD -1  --oneline)
echo "Build master at ${HEAD_SHA1}" > ${LOGFILE}

for i in ${MOUDLES}; do
  build_module $i >> ${LOGFILE} 2>&1
done

if [ "${BUMP_VERSION}" = "true" ]; then
  msg="Bump to ${VERSION}."
  echo -e "\n\n${msg}" >> ${LOGFILE}
  GITMESSAGE="${msg}
${GITMESSAGE}"
  git add .
  git commit -m "${GITMESSAGE}"
  git push origin HEAD:master
  # Do tagging on official build
  if [ "${BUILD_TYPE}" = "official" ]; then
    git tag v${VERSION}
    git push origin v${VERSION}
  fi
fi

echo $GITMESSAGE >> ${LOGFILE}

if [ "${UPLOAD_BUILD}" = "true" ]; then
  # Upload log file
  scp ${LOGFILE} ${REMOTE_USERNAME}@${REMOTE_SERVER}:${ARCHIVE_DIR}/
  # Upload spec
  ssh ${REMOTE_USERNAME}@${REMOTE_SERVER} "mkdir ${ARCHIVE_DIR}/spec"
  scp doc/spec/*.html ${REMOTE_USERNAME}@${REMOTE_SERVER}:${ARCHIVE_DIR}/spec/
  # Ensure permission
  ssh ${REMOTE_USERNAME}@${REMOTE_SERVER} "chgrp -R $REMOTE_USERGROUP ${ARCHIVE_DIR}; chmod -R 775 ${ARCHIVE_DIR}"
  # Create latest symbol link on nightly build
  if [ "${BUILD_TYPE}" = "nightly" ]; then
    ssh ${REMOTE_USERNAME}@${REMOTE_SERVER} "cd ${ARCHIVE_DIR}/..; rm latest; ln -s ${TODAY} latest"
  fi
fi

popd > /dev/null
