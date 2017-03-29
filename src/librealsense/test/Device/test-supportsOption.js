// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

'use strict';

const assert = require('assert');
const addon = require('bindings')('node_librealsense');

describe('Test Device interfaces', function() {
  let context = new addon.Context;
  let device = null;

  before(function(done) {
    context.getDevice(0).then((availableDevice) => {
      device = availableDevice;
      done();
    });
  });

  it('Negative - Call supportsOption with no option', function() {
    return new Promise((resolve, reject) => {
      device.supportsOption().then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call supportsOption with 2 options', function() {
    return new Promise((resolve, reject) => {
      device.supportsOption('color_gain', 'color_hue').then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });

  it('Negative - Call supportsOption with illegal options', function() {
    return new Promise((resolve, reject) => {
      device.supportsOption('dummy').then((enable) => {
        reject('Should get reject but got resolved promise');
      }).catch((e) => {
        resolve();
      });
    });
  });
});

function testOptions(option) {
  describe('Test Device interfaces', function() {
    let context = new addon.Context;
    let device = null;

    before(function(done) {
      context.getDevice(0).then((availableDevice) => {
        device = availableDevice;
        done();
      });
    });

    it('Positive - Call supportsOption with' + option, function() {
      return new Promise((resolve, reject) => {
        device.supportsOption(option).then((enable) => {
          assert.equal(typeof enable, 'boolean');
          resolve();
        }).catch((e) => {
          reject(e);
        });
      });
    });
  });
}

let testSet = [
  'color_backlight_compensation',
  'color_brightness',
  'color_contrast',
  'color_exposure',
  'color_gain',
  'color_gamma',
  'color_hue',
  'color_saturation',
  'color_sharpness',
  'color_white_balance',
  'color_enable_auto_exposure',
  'color_enable_auto_white_balance',
  'f200_laser_power',
  'f200_accuracy',
  'f200_motion_range',
  'f200_filter_option',
  'f200_confidence_threshold',
  'f200_dynamic_fps',
  'sr300_auto_range_enable_motion_versus_range',
  'sr300_auto_range_enable_laser',
  'sr300_auto_range_min_motion_versus_range',
  'sr300_auto_range_max_motion_versus_range',
  'sr300_auto_range_start_motion_versus_range',
  'sr300_auto_range_min_laser',
  'sr300_auto_range_max_laser',
  'sr300_auto_range_start_laser',
  'sr300_auto_range_upper_threshold',
  'sr300_auto_range_lower_threshold',
  'r200_lr_auto_exposure_enabled',
  'r200_lr_gain',
  'r200_lr_exposure',
  'r200_emitter_enabled',
  'r200_depth_units',
  'r200_depth_clamp_min',
  'r200_depth_clamp_max',
  'r200_disparity_multiplier',
  'r200_disparity_shift',
  'r200_auto_exposure_mean_intensity_set_point',
  'r200_auto_exposure_bright_ratio_set_point',
  'r200_auto_exposure_kp_gain',
  'r200_auto_exposure_kp_exposure',
  'r200_auto_exposure_kp_dark_threshold',
  'r200_auto_exposure_top_edge',
  'r200_auto_exposure_bottom_edge',
  'r200_auto_exposure_left_edge',
  'r200_auto_exposure_right_edge',
  'r200_depth_control_estimate_median_decrement',
  'r200_depth_control_estimate_median_increment',
  'r200_depth_control_median_threshold',
  'r200_depth_control_score_minimum_threshold',
  'r200_depth_control_score_maximum_threshold',
  'r200_depth_control_texture_count_threshold',
  'r200_depth_control_texture_difference_threshold',
  'r200_depth_control_second_peak_threshold',
  'r200_depth_control_neighbor_threshold',
  'r200_depth_control_lr_threshold',
];
// eslint-disable-next-line guard-for-in
for(let i in testSet) {
  testOptions(testSet[i]);
}
