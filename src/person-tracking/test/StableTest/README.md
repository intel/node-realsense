## Prepare
* Install gnuplot tool
```
sudo apt install gnuplot
```

## Run RealSense sample test
```
node /path/to/realsense_samples/samples/pt_tutorial_1/javascript/main.js
```

## To get cpu, gpu, memory usage record
```
Usage: sudo ./getUsageRecord.sh <command>
<command>
  command name of running RealSense sample test

sudo ./getUsageRecord.sh node
```

* You'll got message as below and input the test lasting time in second
```
Please input the test lasting time (seconds):
(For example): 3710
```
* Script will start to record the resource and keep data in `*.dat` files

## draw the line chart

```
./drawPng.pg > result.png
```
