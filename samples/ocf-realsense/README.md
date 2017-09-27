# RealSense OCF Demos
These samples illustrate how to develop applications using Intel® RealSense™ JavaScript [API](https://01org.github.io/node-realsense/doc/spec) and Open Connectivity Foundation™ ([OCF](https://openconnectivity.org)) JavaScript [API](https://github.com/01org/zephyr.js/blob/master/docs/ocf.md).

## Architecture

![Image](./doc/sh-ocf-arc.png?raw=true)

## Demos
The following demos are provided in this release.
 - **Control light by distance**(demo1): This sample app illustrates the use of libRealsense, libPT, and the Linux SDK Framework to use the ZR300 camera's depth and color sensors to detect people in the scene. Detected person in the scene will be displayed with the distance information on screen. Meanwhile, the led light will be on and off according to the person's position changing.
 - **Control light/buzzer by person recognition**(demo2)：This sample app illustrates how to register new users to the database, upload the database to identify them when they appear in the scene. Recognized person in the scene will light a green led, otherwise light a red one and open buzzer.

## Get Start

### Hardware
- PC with Ubuntu 16.04
- [MinnowBoard Turbot](https://store.netgate.com/Turbot2.aspx)
- [Intel® RealSense™ Camera ZR300](https://newsroom.intel.com/chip-shots/intel-announces-tools-realsense-technology-development/)
- [Grove Chainable RGB LED](http://www.seeedstudio.com/depot/twig-chainable-rgb-led-p-850.html?cPath=156_157)
- [Grove Buzzer](http://wiki.seeed.cc/Grove-Buzzer/)

### Setup RealSense Execution Environment on Ubuntu

1. Please refer to this [tutorial](https://github.com/01org/node-realsense/blob/master/doc/setup_environment.md) to set up RealSense test environment on Ubuntu.

2. Execute belows commands to start this demo journey:
   ```bash
   # git clone https://github.com/wanghongjuan/ocf-realsense-demos.git
   # cd ocf-realsense-demos
   # export API_SERVER_HOST=<The-OCF-Server-board-ip>
   ```
   Please follow below guide to set up ocf server environment at first, then you can enter directory "demo1" or "demo2", following the corresponding README file to launch the demo. 


### Setup OCF Server Test Environment on MinnowBoard Turbot
1. Please follow this [instruction](https://minnowboard.org/tutorials/installing-ubuntu-16.04-on-minnowboardmax) to install Ubuntu 16.04 on MinnowBoard Turbot.

2. Install Node.js at first, v6.11.2 preferred(https://nodejs.org/en/download/package-manager/)
3. There are some dependencies(uuid-dev, libcure4-openssl-dev and a C++ compiler (gcc-5 or later) etc.) need to install at first, you can install them via command:
   ```bash
   # sudo apt-get install uuid-dev libcurl4-openssl-dev libboost-all-dev sqlite3 libsqlite3-dev glib2.0-dev scons git
   ```
4. Install [`iot-rest-api-server`](https://github.com/01org/iot-rest-api-server) via command `npm install iot-rest-api-server`, for detail information, please refer to [here](https://github.com/01org/iot-rest-api-server/blob/master/README.md). Launch the rest api server:
   ```bash
   # cd node_modules/iot-rest-api-server
   # node index.js &
   ```
5. The other dependence is `libmraa` that Low Level Skeleton Library for Communication on GNU/Linux platforms, you can install it via `npm install mraa`.

6. Please refer to this [guidance](./ocf-servers/README.md#setting-up-the-hw) to setup the sensors on this board

7. Copy the downloaded repo "ocf-realsense-demo/ocf-servers" from the host to this board, then execute below commands to launch all sensors server as administrator(user "root").
   ```bash
   # which node #Remember this node path
   # pwd #Remember this ocf-servers path
   # sudo -i
   # export PATH=$PATH:[path-to-node]
   # cd [path-to-ocf-servers]
   # npm install
   # export NODE_DEBUG=buzzer;node buzzer.js &
   # export NODE_DEBUG=rgb_led;node rgb_led.js &
   ```
   Check these launched server resources whether can be found via iot-rest-api-server
   ```bash
   # cd <path-of-restapi-install>
   # ./test/oic-get /res
   ```
   Then you can find the registered resource `/a/buzzer` and `/a/rgbled`.
