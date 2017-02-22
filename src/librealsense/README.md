# node-librealsense

## Dependencies ##

 - Install [`librealsense` project](https://github.com/IntelRealSense/librealsense.git). This node-addon project depends on that native C++ `librealsense` project
  1. Clone `librealsense` and [setup build environment](https://github.com/IntelRealSense/librealsense#installation-guide)
  1. Build the project `make`.
  1. Install the project `sudo make install`


 - Setup node-addon build environment
  1. [Install node.js](https://nodejs.org/en/download/package-manager/)
  1. `npm install -g node-gyp`
  1. `npm install -g mocha`

## Build ##
```
cd src/librealsense
npm install
```

## APIs ##
There are two main objects exposed in node-librealsense, **Context** and **Device**, which give users the abilities to control the device. Please see the [spec](apis.md) to checkout the support status of node-librealsense compared with [librealsense](https://github.com/IntelRealSense/librealsense).

## Touble Shooting ##

 - Can't connect device, error message:


    terminate called after throwing an instance of 'rs::error'
    what():  UVCIOC_CTRL_QUERY:UVC_SET_CUR error 5, Input/output error
    Aborted (core dumped)


Solution: try unplug RealSense camera and plug it back to computer again.


 - Can't connect device, error message:

`out of range value for argument "index"`

Solution: there is no camera plugged, or the camera is just plugged in for a short amount of time, wait longer.

 - Can't get depth image

Solution: such behavior can be observed sometimes, e.g. a infra-red channel is down and therefore depth channel hangs. You can wait for another while for its self-recovery (yes it sometimes does), or unplug the camera and plug it back again.
