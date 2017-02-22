## BKMs

* Oh my god somebody please help node-gyp build takes too long time it compiles almost all C++ files every time somebody please help me out...

  Use the below shell script to build (with extra benefits)

    ```
    # Workaround of node-gyp build issue
    target=`ls build/*.mk`
    alter=`ls build/*.mk`".new"
    sed 's/widl\ FORCE_DO_CMD/widl/' -r $target > $alter
    cp -f $alter $target
  
    # Build current add-on
    node-gyp build
    
    # Lint tool -- Make your code great again by making lint happy again
    if [ $? -eq 0 ]; then
    	# Save current dir
    	saved=`pwd`
    
    	cd ../../ && python tools/node_modules/widl-nan/tools/lint.py --base=master
    
    	# Test: to do, or not to do
    	if [ $# -gt 0 ] && [ $1 == "test" ]; then
    		cd $saved && mocha
    	else
    		echo "Note: to run test, type '$0 test'"
    	fi
    fi
    ```

* How to debug node addon
```
node-gyp rebuild --debug
gdb --args `which node` sample.js
// In gdb
gdb> b main
gdb> r
gdb> b [SOME_FUNCTION_NAME/LINE_NUMBER]
gdb> c
gdb> l
gdb> s/n
```

* How to debug mocha

```
    >gdb --args `which node` `which mocha`
```

*  ZR300 can't work on Ubuntu
 * Install udev rules
```
sudo cp config/99-realsense-libusb.rules /etc/udev/rules.d/
sudo udevadm control --reload-rules
sudo udevadm trigger
```
 * Above steps should be automatically executed if install librealsense via [PPA](https://code.launchpad.net/~halton-huo/+junk/librealsense).

* Can't connect to camera again if previous run crashed.
 * Current solution: disconnect and reconnect camera from/to your system.
* Crash occurs when addon is running smoothly, and then can't connect to RealSense camera again in a restart
 * The solution: disconnect the camera and then reconnect

Error message of the 1st crash:
```
terminate called after throwing an instance of 'std::runtime_error'
  what():  VIDIOC_DQBUF error 19, No such device
Aborted (core dumped)
```

Error message of later failure to connect:

```
terminate called after throwing an instance of 'rs::error'
  what():  UVCIOC_CTRL_QUERY:UVC_SET_CUR error 5, Input/output error
Aborted (core dumped)
```
 
* If you use add-on directly and has forgotten to inherit from `Node.js` `EventEmitter`, you'll hit the following error (or the like)
```
/usr/bin/nodejs[24777]: ../src/node.cc:1345:v8::Local<v8::Value> node::MakeCallback(node::Environment*, v8::Local<v8::Object>, v8::Local<v8::String>, int, v8::Local<v8::Value>*): Assertion `cb_v->IsFunction()' failed.
 1: node::Abort() [node]
 2: node::Assert(char const* const (*) [4]) [node]
 3: node::MakeCallback(v8::Isolate*, v8::Local<v8::Object>, char const*, int, v8::Local<v8::Value>*) [node]
 4: FutureMakeCallback::TimerCallback(unsigned long) [/home/yunfei/00_yunfei/github/node-realsense/src/object-recognition/build/Release/realsense_object_recognition.node]
 5: WIDLNANTimer::TimerCallbackProc(uv_timer_s*) [/home/yunfei/00_yunfei/github/node-realsense/src/object-recognition/build/Release/realsense_object_recognition.node]
 6: 0x13774f5 [node]
 7: uv_run [node]
 8: node::Start(int, char**) [node]
 9: __libc_start_main [/lib/x86_64-linux-gnu/libc.so.6]
10: _start [node]
Aborted (core dumped)
```
Solution: to inherit from `EventEmitter` or to `require` the `Node.js` wrapper module

* `sudo snap login <your_email>` failed
Edit your `http_proxy`, https_proxy` to /etc/environment, then `sudo service snapd restart`
