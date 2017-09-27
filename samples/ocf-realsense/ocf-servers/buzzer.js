// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

let debuglog = require('util').debuglog('buzzer');
let buzzerResource;
let playNote = false;
let timerId = 0;
let observerCount = 0;
let sensorPin;
let exitId;
let sensorState = false;
let resourceTypeName = 'oic.r.buzzer';
let resourceInterfaceName = '/a/buzzer';
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

// Setup Buzzer sensor pin.
function setupHardware() {
    if (mraa) {
        sensorPin = new mraa.Gpio(12);
        sensorPin.dir(mraa.DIR_OUT);
        sensorPin.write(0);
    }
}

// Buzzer will beep as an alarm pausing
// for 0.8 seconds between.
function playTone() {
    if (playNote)
       sensorPin.write(1);
    else
       sensorPin.write(0);

    playNote = !playNote;
}

// This function parce the incoming Resource properties
// and change the sensor state.
function updateProperties(properties) {
    sensorState = properties.value;

    debuglog('Update received. value: ', sensorState);

    if (simulationMode)
        return;

    if (sensorState) {
        timerId = setInterval(playTone, 300);
    } else {
        if (timerId)
            clearInterval(timerId);

        sensorPin.write(0);
    }
}

// This function construct the payload and returns when
// the GET request received from the client.
function getProperties() {
    // Format the payload.
    let properties = {
        rt: resourceTypeName,
        id: 'buzzer',
        value: sensorState // eslint-disable-line
    };

    debuglog('Send the response. value: ', sensorState);
    return properties;
}

// Set up the notification loop
function notifyObservers(request) {
    buzzerResource.properties = getProperties();

    buzzerResource.notify().then(
        function() {
            debuglog('Successfully notified observers.');
        },
        function(error) {
            debuglog('Notify failed with error: ', error);
        });
}

// Event handlers for the registered resource.
function retrieveHandler(request) {
    buzzerResource.properties = getProperties();
    request.respond(buzzerResource).catch(handleError);

    if ('observe' in request) {
        observerCount += request.observe ? 1 : -1;
        if (observerCount > 0)
            setTimeout(notifyObservers, 200);
    }
}

function updateHandler(request) {
    updateProperties(request.data);

    buzzerResource.properties = getProperties();
    request.respond(buzzerResource).catch(handleError);
    if (observerCount > 0)
        setTimeout(notifyObservers, 200);
}

device.device = Object.assign(device.device, {
    name: 'Smart Home Buzzer',
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

// Enable presence
if (device.device.uuid) {
    // Setup Buzzer sensor pin.
    setupHardware();

    debuglog('Create Buzzer resource.');

    // Register Buzzer resource
    device.server.register({
        id: {path: resourceInterfaceName},
        resourcePath: resourceInterfaceName,
        resourceTypes: [resourceTypeName],
        interfaces: ['oic.if.baseline'],
        discoverable: true,
        observable: true,
        properties: getProperties() // eslint-disable-line
    }).then(
        function(resource) {
            debuglog('register() resource successful');
            buzzerResource = resource;

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
    debuglog('Delete Buzzer Resource.');

    if (exitId)
        return;

    // Stop buzzer before we tear down the resource.
    if (timerId)
        clearInterval(timerId);

    if (mraa)
        sensorPin.write(0);

    // Unregister resource.
    buzzerResource.unregister().then(
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
