# Developer Guide

## Build
To build the addon (incl. auto code generation), simply do the following in console:
```
cd src/object-recognition
npm install
```

## Run

To test the addon
1. Build `librealsense`
1. Connect R200 camera into your system
1. Setup LD_LIBRARY_PATH pointing to the path of all the following
  1. `realsense_sdk` libraries
  1. `opencv` libraries
  1. `librealsense_object_recognition` (middleware) libraries
  1. `librealsense` libraries
1. Copy `classifiers` directory from `librealsense_object_recognition` and put them into ``<project_root>/src` (i.e. the same level with `object-recognition` directory)
1. Run `node examples/quick-test.js`

Please refer to librealsense_object_recognition docs for the required version of `opencv`/`librealsense`/`realsense_sdk`.


## Known Issues
- Hard-coded include & library directories in source (`binding.gyp`)
- LD_LIBRARY_PATH must be set to run the addon
- To enable run addon inside `src/object-recognition` directory, have to put files in `src/classifiers` directory -- expecting native middleware to have an function to configure this runtime behavior
