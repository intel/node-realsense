# Intel® RealSense™ technology with JavaScript API for Linux OS

`node-slam`: Node.js Simultaneous Localization and Mapping (SLAM) module based on Intel® RealSense™ technology.

Please refer to [Release Note](https://github.com/01org/node-realsense/releases) for details of current release.

# Install
Prepare [environment](https://01org.github.io/node-realsense/doc/setup_environment.html) and then execute the following command in shell:
```
npm install --save node-slam
```

# Examples
The following example shows the basic usage of this module. Before executing it, connect an Intel® RealSense™ ZR300 Camera to your computer or developer board.

```
const slamModule = require('node-slam');

slamModule.createInstance().then(slam => {
  slam.on('tracking', (evtData) => {
    // Update on camera pose and etc.
    console.log('Camera tracking': evtData.cameraPose, evtData.accuracy);

    slam.getOccupancyMapUpdate().then((mapData) => {
      // mapData is updated
      // TODO: process mapData.tileCount & etc.
    });

    // Optional: save map to disk
    // slam.saveOccupancyMapAsPpm('occupancy.ppm', true).then(() => {
    //   // Map is saved
    // });
  });

  slam.start().then(() => {
    console.log('Camera + algorithm started...');
  });
});
```

# Document
Please refer to the [project document home page](https://01org.github.io/node-realsense/), and also the details page listed below:
- [Concept and terms](https://software.intel.com/sites/products/realsense/slam/developer_guide.html)
- [Object Module document](https://01org.github.io/node-realsense/doc/spec/slam.html)
- [Common API document](https://01org.github.io/node-realsense/doc/spec/common.html)
