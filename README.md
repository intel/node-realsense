# Intel® RealSense™ technology with JavaScript API for Linux OS

[![Build Status](https://travis-ci.org/01org/node-realsense.svg?branch=master)](https://travis-ci.org/01org/node-realsense)

[ ![License] [license-image] ] [license]

[license-image]: https://img.shields.io/github/license/mashape/apistatus.svg?style=flat
[license]: LICENSE

This project is offering Node.js modules of [Intel® RealSense™ technology](http://www.intel.com/content/www/us/en/architecture-and-technology/realsense-overview.html) including Librealsense, Person Tracaking, Object Recognization, and SLAM capabilities. The target is [Intel® Joule™](https://software.intel.com/en-us/iot/hardware/joule) and ZR300 camera on top of Ubuntu 16.04 DT.

These modules have been registered as NPM packages, listed as below.
  * Librealsense module, [node-librealsense](https://www.npmjs.com/package/node-librealsense)
  * Person Tracking module, [node-person](https://www.npmjs.com/package/node-person)
  * Object Recognition module, [node-object](https://www.npmjs.com/package/node-object)
  * SLAM module, [node-slam](https://www.npmjs.com/package/node-slam)

For example, Person tracking module can be installed by following command.
    
    npm install --save node-person

*[Note] The target platform of these modules is [Intel® Joule™](https://software.intel.com/en-us/iot/hardware/joule) and ZR300 camera on top of Ubuntu 16.04 DT, please prepare the hardware devices and [setup proper environment](https://github.com/01org/node-realsense/blob/master/doc/setup_environment.md) before install and use these modules.*

Besides the [API Specification](./doc/spec) and NPM package site, [JavaScript samples](https://github.com/01org/node-realsense/releases/download/v0.10.0/realsense_samples-0.10.0-node-realsense.tar.gz) are also good examples demonstrating how to use these modules.

## Documents

  * [API Specification](./doc/spec) - This specification describes the `JavaScript` API provided by the `Node.js` C++ add-ons of this repository.
  * [Getting Started](./doc/getting_started.md) - A guide on how to setup build environment, how to build one or many `Node.js` C++ add-on(s) residing in this repository and/or how to use, debug & change API definition to the C++ add-on(s).
  * [Contribution](./doc/contribution.md) - A guide on how to contribute code to this repository.
  * [BKMs](./doc/bkms.md) - Listing Best Known Methods (BKMs) for common issues.

## License
The MIT License (MIT)

Copyright (c) 2016 Intel Corporation. All rights reserved.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
