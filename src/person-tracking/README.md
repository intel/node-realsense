# Intel® RealSense™ technology with JavaScript API for Linux OS

`node-person`: Node.js Person Detection/Person Tracking/Person Recognition/Persong Skeleton Tracking/Body Gesture Recognition/Face Tracking algorithm module based on Intel® RealSense™ technology.

Please refer to [Release Note](https://github.com/01org/node-realsense/releases) for details of current release.

# Install
Prepare [environment](https://01org.github.io/node-realsense/doc/setup_environment.html) and then execute the following command in shell:
```
npm install --save node-person
```

# Examples
The following example shows the basic usage of this module. Before executing it, connect an Intel® RealSense™ Camera to your computer or developer board.

```
const personModule = require('node-person');

personModule.createPersonTracker().then(pt => {
  pt.on('frameprocessed', (evtData) => {
    // A new frame was captured and processed
    // Not necessarily means a person will be always detected
  });

  pt.on('persontracked', (evtData) => {
    // A new frame was processed and then there is person detected
    console.log('Person tracked': evtData);
  });

  // To control how the algorithm works
  // Call method: pt.setPersonTrackerOptions();

  pt.start().then(() => {
    console.log('Camera + algorithm started...');
  });
});
```

# Document
Please refer to the [project document home page](https://01org.github.io/node-realsense/), and also the details page listed below:
- [Concept and terms](https://software.intel.com/sites/products/realsense/person/developer_guide.html#Introduction)
- [Person Module document](https://01org.github.io/node-realsense/doc/spec/person-tracking.html)
- [Common API document](https://01org.github.io/node-realsense/doc/spec/common.html)
