'use strict';

const addon = require('.');

function runObjectRecognition(mode) {
  var options = {
    mode: mode,
    framesSkipping: 10,
    confidenceThreshold: 0.35,
    computeEngine: 'CPU',
    enableSegmentation: true,
    maxReturnObjectCount: 2,
    enableObjectCenterEstimation: false
  };

  addon.createInstance().then((inst) => {
    let instance = inst;
    inst.setCameraOptions({
      color: {
        width: 640,
        height: 480,
        frameRate: 30,
        isEnabled: true
      },
      depth: {
        width: 320,
        height: 240,
        frameRate: 30,
        isEnabled: true
      }
    }).then(() => {
      return instance.setInstanceOptions(options);
    }).then(() => {
      setInterval(function() {
        console.log('  ->instance.state:', instance.state);
      }, 1000);

      instance.on('framecaptured', function () {
        console.log('.captured event');
      });
      instance.on('frameprocessed', function (eventData) {
        console.log('..processed event');
      });
      instance.on('recognition', function (eventData) {
        console.log('...[recognition] event: ', eventData.length);
        eventData.forEach(item => {
          console.log('    :->', item.label, '|', item.probability);
        });
        console.log('----');
      });
      instance.on('localization', function (eventData) {
        console.log('...[localization] event: ', eventData.length);
        eventData.forEach(item => {
          console.log('    :->', item.label, item.probability, item.roi.toString(), item.objectCenter);
        });
        console.log('----');
      });
      return instance.start();
    }).catch((e) => {
      console.log(`Exception caught in promise chain: ${e}`);
    });
  });
}

// runObjectRecognition('localization');
runObjectRecognition('single-recognition');
