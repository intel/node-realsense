# Setup Hardware/Software Environment
- Setup `Intel® RealSense™ SDK for Linux*` development environment, Choose Ubuntu* Operating System

  - [Intel® Joule™ Main Page](https://software.intel.com/en-us/intel-joule-getting-started)
  - [Hardware Setup](https://software.intel.com/en-us/first-time-setup-for-joule)
  - [Flash BIOS](https://software.intel.com/en-us/flashing-the-bios-on-joule)
  - [Operating System Installation](https://software.intel.com/en-us/node/705676)
  - [Package Installation](https://software.intel.com/sites/products/realsense/intro/getting_started.html)
  - [SDK Doc](https://software.intel.com/sites/products/realsense/intro/)


- Install `Node.js*` and its development tools

```
sudo apt-get install nodejs npm
# Workaround of "Node not found" error
sudo ln -s /usr/bin/nodejs /usr/bin/node
# Build tool for Node.js C++ Addon
npm install -g node-gyp
```
