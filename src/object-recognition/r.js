'use strict';

const addon = require('.');
function start_stop_start_test () {
  var instance;
  addon.createInstance().then(i => {
    instance = i;
    return i;
  }).then(i => {
    console.log('>current-state:', instance.state);
    console.log('calling start()');
    return instance.start();
  }).then(() => {
    // assert.equal(instance.state, 'running');
    console.log('>current-state:', instance.state);
    console.log('calling stop()');
    return instance.stop();
  }).then(() => {
    console.log('>current-state:', instance.state);
    // assert.equal(instance.state, 'ready');
    console.log('calling start()');
    return instance.start();
  }).then(() => {
    console.log('>current-state:', instance.state);
    // reject('start got success callback');
    console.log('end of test, everything is fine, just a bug is preventing node.js exiting normally.....');
  }).catch((e) => {
    console.log('something went wrong...', e);
    // if (instance.state === 'paused') {
    //   resolve();
    // }else {
    //   reject(e);
    // }
  });
}

// start_stop_start_test();

function start_must_be_rejected_in_state_running () {
  console.log("===========================================================");
  console.log("start_must_be_rejected_in_state_running");

  let instance;
  let sentry = false;
  return addon.createInstance().then(i => {
    instance = i;
    return i;
  }).then(i => {
    console.log('>current-state:', instance.state);
    console.log('calling start()');
    return instance.start();
  }).then(() => {
    // assert.equal(instance.state, 'running');
    console.log('>current-state:', instance.state);
    console.log('calling start again()');
    sentry = true;
    return instance.start();
  }).then(() => {
    console.log('>current-state:', instance.state);
    console.log('Wrong! It is not supposed to reach here...');
  }).catch((e) => {
    if (sentry) {
      console.log('>current-state:', instance.state);
      console.log(`We are good here with the exception [${e}] because we are expecting the rejection`);
    }
    instance.stop();
  });
}

function start_must_be_rejected_in_state_paused () {
  console.log("===========================================================");
  console.log("start_must_be_rejected_in_state_paused");

  let instance;
  let sentry = false;
  return addon.createInstance().then(i => {
    instance = i;
    return i;
  }).then(i => {
    console.log('>current-state:', instance.state);
    console.log('calling start()');
    return instance.start();
  }).then(() => {
    // assert.equal(instance.state, 'running');
    console.log('>current-state:', instance.state);
    console.log('calling pause()');
    return instance.pause();
  }).then(() => {
    console.log('>current-state:', instance.state);
    console.log('calling start again()');
    sentry = true;
    return instance.start();
  }).then(() => {
    console.log('>current-state:', instance.state);
    console.log('Wrong! It is not supposed to reach here...');
  }).catch((e) => {
    if (sentry) {
      console.log('>current-state:', instance.state);
      console.log(`We are good here with the exception [${e}] because we are expecting the rejection`);
    }
    instance.stop();
  });
}

function stop_must_work_in_paused_state () {
  console.log("===========================================================");
  console.log("stop_must_work_in_paused_state");

  let instance;
  return addon.createInstance().then(i => {
    instance = i;
    instance.on('recognition', function () {
      console.log('[recognition] event fired!');
    });
    return i;
  }).then(i => {
    console.log('>current-state:', instance.state);
    console.log('calling start()');
    return instance.start();
  }).then(() => {
    // assert.equal(instance.state, 'running');
    console.log('>current-state:', instance.state);
    console.log('calling pause()');
    return instance.pause();
  }).then(() => {
    console.log('>current-state:', instance.state);
    setTimeout(function() {
      console.log('calling stop()');
      instance.stop().then(function () {
        console.log('>current-state:', instance.state);
        console.log('end of test, everything is fine, just a bug is preventing node.js exiting normally.....');
      });
    }, 5000);
  }).then(() => {
    //
  }).catch((e) => {
    if (sentry) {
      console.log('>current-state:', instance.state);
      console.log('Wrong! It is not supposed to reach here...');
    }
    instance.stop();
  });
}

// start_must_be_rejected_in_state_running();
// start_must_be_rejected_in_state_paused();
// stop_must_work_in_paused_state();


function no_crash_call_start_stop_same_time () {
  console.log("===========================================================");
  console.log("no_crash_call_start_stop_same_time");

  let instance;
  return addon.createInstance().then(i => {
    instance = i;
    instance.on('recognition', function (e) {
      console.log('[recognition] event fired!');
      // console.log('[recognition] event fired!', e);
    });
    return i;
  }).then(i => {
    console.log('>current-state:', instance.state);
    console.log('calling start()');
    console.log('>current-state:', instance.state);
    console.log('calling stop() without waiting for the promise');
    let p = instance.start();
    instance.stop().then(function () {
      console.log("Not expected!!!");
    }).catch(function () {
      console.log("Expected rejection");
    });
    console.log('>current-state:', instance.state);
    return p;
  }).then(() => {
    console.log('calling stop()');
    return instance.stop();
  }).then(() => {
    console.log('>current-state:', instance.state);
    console.log('end of test, everything is fine, just a bug is preventing node.js exiting normally.....');
  }).catch((e) => {
  });
}

no_crash_call_start_stop_same_time();
