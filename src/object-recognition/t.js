'use strict';

const addon = require('.');

function quick_test() {
  let or_instance;
  addon.createInstance().then(instance => {
    console.log('1. Creating instance');
    console.log(instance);

    let firstEventTimestamp = undefined;
    console.log('2. Setup event listeners');
    instance.on('recognition', function (data) {
      console.log('New [recognition] result is caught: ', data);
      // instance.getFrameData().then((info) => {
      //   let currentTimestamp = new Date().getTime();
      //   if (!firstEventTimestamp) {
      //     firstEventTimestamp = currentTimestamp;
      //   }
      //   const delta = currentTimestamp - firstEventTimestamp;
      //   console.log('>JavaScript incremental time in millisecond', delta);
      //   console.log(info);
      //   console.log('>Diff between JavaScript time & frameNumber:', info.depth.frameNumber - delta);
      // });
    });
    or_instance = instance;
    return instance.setInstanceOptions({mode: 'localization', enableObjectCenterEstimation: true});
  }).then(function() {
    console.log('3. Start RealSense camera');
    return or_instance.start();
  }).then(function() {
    console.log('Camera started...');
  }).catch(e => {
    console.log(`Something was wrong ${e}`);
  });
}

quick_test();

