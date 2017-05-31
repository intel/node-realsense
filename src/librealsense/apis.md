### Context
Interfaces of librealsense| node-librealsense support | Description
------------ | :-------------: | -------------
getDevice | √ |  retrieve connected device by index
getDeviceCount | √ | determine number of connected devices

### Device
Interfaces of librealsense| node-librealsense support | Description
------------ | :-------------: | -------------
getName | √ |  retrieve a human readable device model
getSerial | √ | retrieve the unique serial number
getFirmwareVersion | √ | retrieve the version of the firmware
getDepthScale | √ | retrieve mapping between the units of the depth image and meters
isStreaming | √ | determine if the device is currently streaming
supportsOption | √ | determine if the device allows a specific option to be queried and set
getStreamModeCount | √ | determine the number of streaming modes available for a given stream
getStreamMode | √ | determine the properties of a specific streaming mode
isStreamEnabled | √ | determine if a specific stream is enabled
enableStream | √ | enable a specific stream
getStreamWidth | √ | retrieve the width in pixels of a specific stream
getStreamHeight | √ | retrieve the height in pixels of a specific stream
getFrameTimeStamp | √ | retrieve the time at which the frame was captured
start | √ | begin streaming on all enabled streams for this device
stop | √ | end streaming on all streams for this device
getFrameData | √ (supported by event) | retrieve the contents of the latest frame on a stream
getUSBPortId | √ | retrieve the USB port number
getInfo | √ | retrieve camera specific information like the versions of the various componnents
getExtrinsics | √ | retrieve extrinsic transformation between the viewpoints of two different streams
getMotionExtrinsicsFrom | √ | retrieve extrinsic transformation between the viewpoints of specific stream and the motion module
disableStream | √ | disable a specific stream
getStreamFormat | √ | retrieve the pixel format for a specific stream
getStreamFramerate | √ | retrieve the framerate for a specific stream
getStreamIntrinsics | x | retrieve intrinsic camera parameters for a specific stream
getMotionIntrinsics | x | retrieve intrinsic camera parameters for the motion module
getOptionRange | x | retrieve the available range of values of a supported option
getOption(s) | x | retrieve the value of an arbitrary number of options
setOptions(s) | x | set the value of an arbitrary number of options
getOptionDescription | x | retrieve the device specific option description
supports | x | determine device capabilities
getFrameNumber | √ | retrieve the time at which the latest frame on a stream was captured
sendBlobToDevice | x |send device specific data to the device
enableMotionTracking | x | enable motion module event
disableMotionTracking | x | disable motion module event
isMotionTrackingActive | x | check if data acquisition is active
