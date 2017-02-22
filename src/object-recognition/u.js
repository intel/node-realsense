'use strict';

const addon = require('.');

function quick_test() {
  let instance = null;
  addon.createInstance().then(inst => {
    console.log('1. Creating instance');
    instance = inst;
    console.log(instance);

    return instance.setInstanceOptions({enableObjectCenterEstimation: true});
  }).then(function() {
    let firstEventTimestamp = undefined;
    console.log('2. Setup event listeners');
    instance.on('recognition', function (data) {
      console.log('New [recognition] result is caught: ', data);
      instance.getFrameData().then((info) => {
        let currentTimestamp = new Date().getTime();
        if (!firstEventTimestamp) {
          firstEventTimestamp = currentTimestamp;
        }
        const delta = currentTimestamp - firstEventTimestamp;
        console.log('>JavaScript incremental time in millisecond', delta);
        console.log(info);
        // console.log('>Diff between JavaScript time & frameNumber:', info.depth.frameNumber - delta);
      });
    });

    console.log('3. Start RealSense camera');
    return instance.start();
  }).then(function() {
    console.log('Camera started...');
  }).catch(e => {
    console.log(`Something was wrong ${e}`);
  });
}

quick_test();
