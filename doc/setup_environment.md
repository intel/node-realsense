# Setup Hardware/Software/Build Environment

It takes 4 steps to setup the entire development environment: Hardware Platform, Intel® RealSense™ Camera, Operating System and Build Environment.

1. Hardware Platform: you can choose to use a Linux PC or a Linux IoT board (e.g. [Intel® Joule™ Module](https://software.intel.com/en-us/iot/hardware/joule)).
    - If you choose to use a Linux PC, please go to step 2.
    - If you choose an IoT board, you need to setup the hardware platform. Taking Intel® Joule™ Module for example, goto [online guide](https://software.intel.com/en-us/node/721460) for platform setup information. 
1. Intel® RealSense™ camera: you need an [Intel® RealSense™](http://www.intel.com/content/www/us/en/architecture-and-technology/realsense-overview.html) camera to explore the full capabilities provided by this repository.
    - For SLAM, Object Library and Person Library, you can use an [Intel® RealSense™ Camera ZR300](https://newsroom.intel.com/chip-shots/intel-announces-tools-realsense-technology-development/) that is currently available for [pre-order online](https://click.intel.com/intelr-realsensetm-development-kit-featuring-the-zr300.html).
    - For Object Library and Person Library, you can also use an [Intel® RealSense™ Camera R200](https://software.intel.com/en-us/realsense/r200camera), which is available in multiple online stores, e.g. [Newegg purchase link](https://www.newegg.com/Product/Product.aspx?Item=9SIA7HN5J45184&ignorebbr=1&nm_mc=KNC-GoogleMKP-PC&cm_mmc=KNC-GoogleMKP-PC-_-pla-_-Motherboard+%2F+CPU+%2F+VGA+Sets-_-9SIA7HN5J45184&gclid=CKuoiPSi-tICFUtNfgodmPkPCw&gclsrc=aw.ds)
    - (Optional): This [Intel® RealSense™ Cross Platform API](https://github.com/IntelRealSense/librealsense) repository provides [Installation Guide](https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md) including a [touble shooting section](https://github.com/IntelRealSense/librealsense/blob/master/doc/installation.md#troubleshooting-installation-and-patch-related-issues).
1. Operating System: Ubuntu 16.04 LTS is recommended.
    - If you choose to use a Linux PC, make sure you have Ubuntu 16.04 installed.
    - If you choose to use an Intel® Joule™ Module, following this [Installation Guide](https://developer.ubuntu.com/core/get-started/intel-joule#alternative-install:-ubuntu-desktop-16.04-lts) to install Ubuntu Operating System. Make sure you choose `Alternative install: Ubuntu Desktop 16.04 LTS`. A direct (but temporary) link to [*.iso image](http://people.canonical.com/~platform/snappy/tuchuck/desktop-beta4/tuchuck-xenial-desktop-iso-20170109-0.iso).
      - Please be noted that Intel® Joule™ Module might need a [BIOS update](https://software.intel.com/en-us/flashing-the-bios-on-joule) before installing an Operating System ([firmware v174](https://downloadmirror.intel.com/26206/eng/Joule-Firmware-2016-12-18-174-Public.zip) or above)
1. Build Environment: to install Intel® RealSense™ SDK for Linux, `Node.js` and necessary tool(s) for `Node.js` C++ add-on development.
    - Install Intel® RealSense™ SDK for Linux by following instructions listed in [Getting Started Guide](https://software.intel.com/sites/products/realsense/intro/getting_started.html)
    - Install `Node.js` and build tools.

```
# You know what they are
sudo apt-get install nodejs npm

# An optional workaround of "Node not found" error
sudo ln -s /usr/bin/nodejs /usr/bin/node

# Install the build tool for Node.js C++ add-on
npm install -g node-gyp
```
