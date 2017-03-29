# Intel® RealSense™ technology with JavaScript API for Linux OS

`node-librealsense`: Node.js librealsense module based on Intel® RealSense™ technology.

Please refer to [Release Note](https://github.com/01org/node-realsense/releases) for details of current release.

# Install
Prepare [environment](https://01org.github.io/node-realsense/doc/setup_environment.html) and then execute the following command in shell:
```
npm install --save node-librealsense
```

# Examples
The following example shows the basic usage of this module. Before executing it, connect an Intel® RealSense™ Camera to your computer or developer board.

```
const module = require('node-librealsense');

let context = new module.Context;

context.getDevice(0).then((device) => {
  let mode = {
    width: 640,
    height: 480,
    format: 'z16',
  };

  device.on('frameready', (frame) => {
    // A new frame was captured.
  });

  device.enableStream('depth', mode).then(() => {
    device.start().then(() => {
      console.log('Camera started...');
    });
  });
});

```

# Document
Please refer to the [project document home page](https://01org.github.io/node-realsense/), and also the details page listed below:
- [Intel® RealSense™ Cross Platform API](https://github.com/IntelRealSense/librealsense)
