# Setup Hardware/Software Environment
> 1) Purchase hardwares _(please refer to the bottom of this page)_

> 2) Flash BIOS and install Operating System. [Flashing BIOS Guide](https://software.intel.com/en-us/flashing-the-bios-on-joule). **TODO**: OS installation guide

> 3) Install `Node.js*`
```
sudo apt-get install nodejs npm
# Workaround of "Node not found" error
sudo ln -s /usr/bin/nodejs /usr/bin/node
```

> 4) Install dependencies: `Intel® RealSense™ SDK for Linux*`

> 4.1) Clean up environment (optional step - if previously installed)

> 4.2) Install debian packages **TODO**: put more details

## Hardware Requirement & Purchase Link
Name              | Purchase Link | Comment
------------------| --------------| -------
[Intel® Joule™](https://software.intel.com/en-us/iot/hardware/joule) | [Newegg*](http://www.newegg.com/Product/Product.aspx?Item=N82E16813121832) |
ZR300 Camera | | For Object Recognition, Person Tracking & SLAM
[R200 Camera](https://software.intel.com/en-us/realsense/r200camera) | [Newegg*](http://www.newegg.com/Product/Product.aspx?Item=N82E16813121834&cm_re=joule-_-13-121-834-_-Product) | For Object Recognition & Person Tracking, but NOT SLAM
12V/3A AC Adapter |  |
USB Thumb Drive | | To install Operating System
Mico HDMI Cable | | To Connect to Monitor
Keyboard & Mouse | |
(optional) USB 3.0 Hub with power supply | | To connect RealSense Camera, Keyboard&Mouse
(optional) USB to RJ45 adapter | | To connect to wired LAN
(optional) USB-C to USB | | Shipped with Joule
(optional) SD Card | | Shipped with Joule
(optional) High Speed SD Card | | If OS is installed on SD card
