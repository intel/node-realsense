// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.
// limitations under the License.

let debuglog = require('util').debuglog('rgb_led');
let rgbLEDResource;
let exitId;
let observerCount = 0;
let resourceTypeName = 'oic.r.colour.rgb';
let resourceInterfaceName = '/a/rgbled';
let range = [0, 255];
let rgbValue = [0, 0, 0];
let clockPin;
let dataPin;
let simulationMode = false;
let secureMode = true;

// Parse command-line arguments
let args = process.argv.slice(2);
args.forEach(function(entry) {
    if (entry === '--simulation' || entry === '-s') {
        simulationMode = true;
        debuglog('Running in simulation mode');
    } else if (entry === '--no-secure') {
        secureMode = false;
    }
});

// Create appropriate ACLs when security is enabled
if (secureMode) {
    debuglog('Running in secure mode');
    require('./config/json-to-cbor')(__filename, [{
        href: resourceInterfaceName, // eslint-disable-line
        rel: '', // eslint-disable-line
        rt: [resourceTypeName], // eslint-disable-line
        'if': ['oic.if.baseline'] // eslint-disable-line
    }], true);
}

let device = require('iotivity-node');

// Require the MRAA library
let mraa = '';
if (!simulationMode) {
    try {
        mraa = require('mraa');
    } catch (e) {
        debuglog('No mraa module: ', e.message);
        debuglog('Automatically switching to simulation mode');
        simulationMode = true;
    }
}

// Setup LED pin.
function setupHardware() {
    if (!mraa)
        return;

    clockPin = new mraa.Gpio(16);
    clockPin.dir(mraa.DIR_OUT);
    dataPin = new mraa.Gpio(14);
    dataPin.dir(mraa.DIR_OUT);

    setColourRGB(0, 0, 0);
}

function clk() {
    if (!mraa)
        return;

    clockPin.write(0);
    clockPin.write(1);
}

function sendByte(b) {
    if (!mraa)
        return;

    // send one bit at a time
    for (let i = 0; i < 8; i++) {
        if ((b & 0x80) != 0)
            dataPin.write(1);
        else
            dataPin.write(0);

        clk();
        b <<= 1;
    }
}

function sendColour(red, green, blue) {
    // start by sending a byte with the format '1 1 /B7 /B6 /G7 /G6 /R7 /R6'
    let prefix = 0xC0;

    if ((blue & 0x80) == 0) prefix |= 0x20;
    if ((blue & 0x40) == 0) prefix |= 0x10;
    if ((green & 0x80) == 0) prefix |= 0x08;
    if ((green & 0x40) == 0) prefix |= 0x04;
    if ((red & 0x80) == 0) prefix |= 0x02;
    if ((red & 0x40) == 0) prefix |= 0x01;

    sendByte(prefix);

    sendByte(blue);
    sendByte(green);
    sendByte(red);
}

// Set the RGB colour
function setColourRGB(red, green, blue) {
    // send prefix 32 x '0'
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);

    sendColour(red, green, blue);

    // terminate data frame
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
}

function checkColour(colour) {
    let min = range[0];
    let max = range[1];

    if (colour >= min && colour <= max)
        return true;

    return false;
}

// This function parce the incoming Resource properties
// and change the sensor state.
function updateProperties(properties) {
    let input = properties.rgbValue;
    if (!input)
        return;

    let r = parseInt(input[0]);
    let g = parseInt(input[1]);
    let b = parseInt(input[2]);
    if (!checkColour(r) || !checkColour(g) || !checkColour(b))
        return;

    if (!simulationMode)
        setColourRGB(r, g, b);
    rgbValue = input;

    debuglog('Update received. value: ', rgbValue);
}

// This function construct the payload and returns when
// the GET request received from the client.
function getProperties() {
    // Format the payload.
    let properties = {
        rt: resourceTypeName,
        id: 'rgbled',
        rgbValue: rgbValue,
        range: range // eslint-disable-line
    };

    debuglog('Send the response. value: ', rgbValue);
    return properties;
}

// Set up the notification loop
function notifyObservers(request) {
    rgbLEDResource.properties = getProperties();

    rgbLEDResource.notify().catch(
        function(error) {
            debuglog('Notify failed with error: ', error);
        });
}

// Event handlers for the registered resource.
function retrieveHandler(request) {
    rgbLEDResource.properties = getProperties();
    request.respond(rgbLEDResource).catch(handleError);

    if ('observe' in request) {
        observerCount += request.observe ? 1 : -1;
        if (observerCount > 0)
            setTimeout(notifyObservers, 200);
    }
}

function updateHandler(request) {
    updateProperties(request.data);

    rgbLEDResource.properties = getProperties();
    request.respond(rgbLEDResource).catch(handleError);
    if (observerCount > 0)
        setTimeout(notifyObservers, 200);
}

device.device = Object.assign(device.device, {
    name: 'Smart Home RGB LED',
    coreSpecVersion: 'core.1.1.0',
    dataModels: ['res.1.1.0'] // eslint-disable-line
});

function handleError(error) {
    debuglog('Failed to send response with error: ', error);
}

device.platform = Object.assign(device.platform, {
    manufacturerName: 'Intel',
    manufactureDate: new Date('Fri Oct 30 10:04:17 (EET) 2015'),
    platformVersion: '1.1.0',
    firmwareVersion: '0.0.1' // eslint-disable-line
});

if (device.device.uuid) {
    // Setup RGB LED sensor pin.
    setupHardware();

    debuglog('Create RGB LED resource.');

    // Register RGB LED resource
    device.server.register({
        resourcePath: resourceInterfaceName,
        resourceTypes: [resourceTypeName],
        interfaces: ['oic.if.baseline'],
        discoverable: true,
        observable: true,
        properties: getProperties() // eslint-disable-line
    }).then(
        function(resource) {
            debuglog('register() resource successful');
            rgbLEDResource = resource;

            // Add event handlers for each supported request type
            resource.onretrieve(retrieveHandler);
            resource.onupdate(updateHandler);
        },
        function(error) {
            debuglog('register() resource failed with: ', error);
        });
}

// Cleanup when interrupted
function exitHandler() {
    debuglog('Delete RGB LED Resource.');

    if (exitId)
        return;

    // Turn off led before we tear down the resource.
    if (mraa) {
        rgbValue = [0, 0, 0];
        setColourRGB(0, 0, 0);
    }

    // Unregister resource.
    rgbLEDResource.unregister().then(
        function() {
            debuglog('unregister() resource successful');
        },
        function(error) {
            debuglog('unregister() resource failed with: ', error);
        });

    // Exit
    exitId = setTimeout(function() {
        process.exit(0);
    }, 1000);
}

// Exit gracefully
process.on('SIGINT', exitHandler);
process.on('SIGTERM', exitHandler);
