# Intel® RealSense™ technology with JavaScript API for Linux OS

`node-object`: Node.js Object Recognition/Localization/Tracking algorithm module based on Intel® RealSense™ technology.

Please refer to [Release Note](https://github.com/01org/node-realsense/releases) for details of current release.

# Install
Prepare [environment](https://01org.github.io/node-realsense/doc/setup_environment.html) and then execute the following command in shell:
```
npm install --save node-object
```

# Examples
The following example shows the basic usage of this module. Before executing it, connect an Intel® RealSense™ Camera to your computer or developer board.

```
const objectModule = require('node-object');

objectModule.createObjectRecognizer().then(or => {
  or.on('framecaptured', (evtData) => {
    // A new frame was captured
  });

  or.on('frameprocessed', (evtData) => {
    // A new frame was captured and processed
  });

  or.on('recognition', (evtData) => {
    // A new frame was captured, processed and then recognized
    console.log('New object recognized': evtData);
  });

  // To control how the algorithm works
  // Call method: or.setObjectRecognitionOptions();

  or.start().then(() => {
    console.log('Camera + algorithm started...');
  });
});
```

# Document
Please refer to the [project document home page](https://01org.github.io/node-realsense/), and also the details page listed below:
- [Concept and terms](https://software.intel.com/sites/products/realsense/object/developer_guide.html)
- [Object Module document](https://01org.github.io/node-realsense/doc/spec/object-recognition.html)
- [Common API document](https://01org.github.io/node-realsense/doc/spec/common.html)
