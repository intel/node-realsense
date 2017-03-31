# Getting Started

This document will get you started with all `npm` modules residing in this repository. You will be able to build all `Node.js` module/add-on on top of your `Linux` PC or `IoT` board, and then make your customization to the C++ add-on(s).

# Preparing Build Environment

Make sure you've done all the necessary steps described in [Setup Environment](./setup_environment.md), to get your hardware and software environment ready.

# Build C++ Add-on
There is 4 `Node.js` C++ add-ons in this repository. `Node.js` C++ add-on is supposed to be built by `node-gyp`, which should be ready installed in [Setup Environment](./setup_environment.md).

Choose the add-on that you want to work on, by typing: `cd src/<add-on directory>`, e.g. taking `Person Library` as an example:

```
cd src/person-tracking
```

And then key in

```
npm install
node-gyp rebuild
```

to build the add-on. You might see several lines of message. The build is successful if it ends with the following lines.

```
make: Leaving directory '<your local dir>/src/person-tracking/build'
gyp info ok
```

# Use C++ Add-on in Node.js Program
After building it, it's time to test it in your `Node.js` program.  Unlike the standard `npm install <module-name>` way of using a module/add-on, you need to explicitly specify the directory in your `Node.js` program, e.g.

```
// Node.js program
const personLibrary = require('</path/to/your/code>/src/person-tracking');
```

# Debug C++ Add-on

To debug the C++ add-on, the add-on should be built with debug symbols:

```
node-gyp rebuild --debug
```

After a successful debug build, you can use `gdb` or other debugger to debug the C++ source code, e.g.

```
gdb --args node <your-program>.js
```

# Run Unit Test

`mocha` is the framework for unit testing, simply key in the following command to run the unit test.

```
mocha
```

Then the test will be started and test report will be printed in terminal.

To specify a test suite to run, key in the following command:

```
mocha test/<file-name>.js
```

# Change API Definition

The API of each C++ add-on is defined by `WebIDL` language and then language bindings will be generated using [`WIDL-NAN`](https://www.npmjs.com/package/widl-nan).

To change API definition, go to `src/idl` directory and choose a file to modify, and then implement the change in C++ source code. Run `node-gyp build` in your terminal to see if it builds.

Checkout [`WIDL-NAN` Wiki](https://github.com/01org/widl-nan/wiki) for the design and usage of the `WIDL-NAN` language binding tool.


# Contributing Code

After making your change, you might wat to contribute the change into our repo.
Pleaes refer to [Contribution Guide](./contribution.md) for more information.

# Misc
- You might want to read the [Best Known Methods (BKM) document](./bkms.md) to addressing known issues.
- Current API definition is described in [specification](https://01org.github.io/node-realsense/doc/spec/).

# Troubleshooting
- `rs::error` with `what(): uvcvideo kernel module is not loaded`

    The uvcvideo module needed to be loaded into the kernal.
    ```
    sudo modprobe uvcvideo
    ```
- Error message says missing `boostXXX` file during building process
    Need to install used libboost library as the following line.
    ```
    sudo apt-get install libboost-thread-dev
    ```
