var addon = require('bindings')('realsense_object_recognition');
var EventEmitter = require('events').EventEmitter;

function inherits(target, source) {
  for (var k in source.prototype) {
    target.prototype[k] = source.prototype[k];
  }
}

inherits(addon.ObjectRecognizer, EventEmitter);
// console.log(addon.ObjectRecognizer.prototype);


function test1() {
  var options = {
    mode: 'single-recognition',
    confidenceThreshold: 0.35,
    computeEngine: 'CPU',
    enableSegmentation: true,
    maxReturnObjectCount: 3,
    enableObjectCenterEstimation: true
  };
  var instance = undefined;

  addon.createInstance().then(inst => {
    instance = inst;

    console.log('1. Creating instance');
    console.log(instance);

    console.log('2. Setup event listeners');
    instance.on('recognition', function (evt) {
      console.log('New event: ', evt);
    });

    return instance.setInstanceOptions(options);
  }).then(function () {
    console.log('3. Start RealSense camera');
    return instance.start();
  }).then(function() {
    console.log('...........................started');
    setTimeout(function() {
      console.log('trying to pause');
      instance.pause().then(function(){
        console.log('...........................paused');
      });
    }, 8000);
    setTimeout(function() {
      console.log('trying to resume');
      instance.resume().then(function(){
        console.log('...........................resumed');
      });
    }, 10000);
    setTimeout(function() {
      console.log('trying to stop');
      instance.stop().then(function(){
        console.log('...........................stopped');
      });
    }, 15000);
  }).catch(e => {
    console.log(`Something was wrong ${e}`);
  });
}

function test2() {
  var options = {
    mode: 'localization',
    confidenceThreshold: 0.35,
    computeEngine: 'CPU',
    enableSegmentation: true,
    maxReturnObjectCount: 3,
    enableObjectCenterEstimation: true
  };
  var instance = undefined;

  addon.createInstance().then(inst => {
    instance = inst;

    console.log('1. Creating instance');
    console.log(instance);

    console.log('2. Setup event listeners');
    instance.on('localization', function (evt) {
      console.log('New event: ', evt.label, evt.probability, evt.objectCenter, evt.roi.toString());
      // instance.getFrameData().then(function (data) {
      //   console.log(data);
      //   console.log('Color frame buffer: ', data.getColorFrame());
      //   console.log('Depth frame buffer: ', data.getDepthFrame());
      // });
    });

    return instance.setInstanceOptions(options);
  }).then(function () {
    console.log('3. Start RealSense camera');
    return instance.start();
  }).then(function() {
    console.log('Camera started...');
    // setTimeout(function() {
    //   instance.pause();
    // }, 8000);
    // setTimeout(function() {
    //   instance.resume();
    // }, 16000);
  }).catch(e => {
    console.log(`Something was wrong ${e}`);
  });
}

test1();

