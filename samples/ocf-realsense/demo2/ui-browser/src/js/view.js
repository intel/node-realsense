// Copyright (c) 2016 Intel Corporation. All rights reserved.
// Use of this source code is governed by a MIT-style license that can be
// found in the LICENSE file.

const ViewStatus = {
    DISCONNECTED: 0,
    CONNECTING: 1,
    CONNECTED: 2 // eslint-disable-line
};

let viewModel = {
    S: ViewStatus,
    status: ViewStatus.DISCONNECTED,
    fisheyeEnabled: false,
    colorEnabled: false,
    pose: {x: 0, y: 0, z: 0},
    tracking: -1,
    wsurl: '' // eslint-disable-line
};

const TrackingStatus = [ // eslint-disable-line
    {code: 0, text: 'fail'},
    {code: 1, text: 'low'},
    {code: 2, text: 'medium'},
    {code: 3, text: 'high'},
    {code: 4, text: 'reset'} // eslint-disable-line
];

const maxTableRows = 40;
let view = new Vue({ // eslint-disable-line
    el: '#poseview',
    data: viewModel,
    computed: {
        isConnected: function() {
            return this.status === ViewStatus.CONNECTED;
        },
    },
    watch: {
        'colorEnabled': function(val) {
            console.log('subscribing to color: ' + val);
            transporter.sendMessage({
                type: 'control',
                command: 'color',
                subscribe: !!val // eslint-disable-line
            });
        } // eslint-disable-line
    },
    methods: {
        check: () => {
            if(document.getElementById('tButtonName').innerText == 'Resume') {
                console.log('Calling transporter open');
                transporter.open();
            } else {
                console.log('Calling transporter close');
                transporter.close();
            }
        },
        loadDB: () => {
            transporter.sendMessage({type: 'control', command: 'load_pt_db'});
        },
        stop: () => {
            transporter.close();
        } // eslint-disable-line
    } // eslint-disable-line
});

function getHostname() {
    let saved = localStorage.getItem('host');
    return (!!saved) ? saved : 'ws:// '+location.host;
}
const transporter = new SpTransport(getHostname());

function Log(tag) { // eslint-disable-line
    return function(arg) {
        // rec.save(tag, arg);
        console.log(tag, arg);
    };
}

function resetAll() {
}

transporter.onOpen = () => {
    console.log('connected');
    viewModel.wsurl = transporter._ws.url;
    viewModel.status = ViewStatus.CONNECTED;
    document.getElementById('tButtonName').innerHTML = 'Pause';
};

transporter.onClose = () => {
    console.log('disconnected');
    viewModel.wsurl = '';
    viewModel.status = ViewStatus.DISCONNECTED;

    document.getElementById('tButtonName').innerHTML = 'Resume';
};

transporter.onEvent = (event, msg) => {
    console.log(event, msg);
    if (event === 'on_reset_completed') {
        console.log('got reset event...');
        resetAll();
    }
};

let rgbView = new BufferViewer();
rgbView.init(document.querySelector('#color-canvas'));
let rgbRenderCall = rgbView.render.bind(rgbView);

overlayCanvas = document.getElementById('color-2d-overlay');
ctx2d = overlayCanvas.getContext('2d');
overlayCanvas.addEventListener('click', onOverlayCanvasClick, false);

this.prevX=0.0;
this.prevY=0.0;
this.prevZ=0.0;
this.prevOrientation='';
this.prevPtResultList = [];
transporter.onPTDataUpdate = (timestamp, ptData) => {
    let trackingResult=false;
    let headPoseResult=false;
    let orientationResult=false;
    let gestureResult=false;
    if (ptData === undefined) {
        console.log('The PT data is not available');
    } else {
        let scaleFactor = 2;

        let canvas2d = document.getElementById('color-2d-overlay');
        if (canvas2d == null) {
            console.log('Failed to get color-2d-overlay element');
        }

        let ctx2d = canvas2d.getContext('2d');
        if (ctx2d == null) {
            console.log('Failed to get 2d context from color-2d-overlay element');
        }

        ctx2d.clearRect(0, 0, canvas2d.width, canvas2d.height);    // Clear the overlay canvas

        ctx2d.lineWidth = 2;

        ctx2d.font = '11px sans-serif';
        ctx2d.fillStyle = 'rgba(255, 255, 0, 1.0)';        // Color of text - yellow
        ctx2d.strokeStyle = 'rgba(250, 50, 50, 1.0)';      // Color of stroke rectangle - red
        ctx2d.shadowColor = 'rgba(0, 0, 0, 0.5)';
        ctx2d.shadowOffsetX = 2;
        ctx2d.shadowOffsetY = 3;
        ctx2d.shadowBlur = 4;

        let ptResultList = ptData.Object_result;
        // Remember for click on canvas
        this.prevPtResultList = ptResultList;

        for (let i = 0; i < ptResultList.length; i++) {
            let ptObject = ptResultList[i];

            let dataFinal = getCurrentTime(timestamp) + ';' + 'pid: ' + ptObject.pid;
            console.log('pid is: ' + ptObject.pid);

            let rid = undefined;

            if(ptObject.hasOwnProperty('rid')) {
                 rid = ptObject.rid;
                 dataFinal += ', rid: ' + rid;
            }

            console.log('rid is: ' + rid);

            // PT Sample 1 result
            if(ptObject.hasOwnProperty('center_mass_world')) {
                let CenterMassWorldX = ptObject.center_mass_world.x.toFixed(4);
                let CenterMassWorldY = ptObject.center_mass_world.y.toFixed(4);
                let CenterMassWorldZ = ptObject.center_mass_world.z.toFixed(4);

                dataFinal += ';' + CenterMassWorldX + ', ' + CenterMassWorldY + ', ' +
                              CenterMassWorldZ+ ';';

                // To Reduce print overflow
                if( Math.abs(this.prevX-CenterMassWorldX)>0.3 ||
                    Math.abs(this.prevY-CenterMassWorldY)>0.3 ||
                    Math.abs(this.prevZ-CenterMassWorldZ)>0.3) {
                    this.prevX=CenterMassWorldX;
                    this.prevY=CenterMassWorldY;
                    this.prevZ=CenterMassWorldZ;
                    trackingResult = true;
                }
            }

            if(ptObject.hasOwnProperty('cumulative_total')) {
                dataFinal += ptObject.cumulative_total;
            }

            if(ptObject.hasOwnProperty('person_bounding_box')) {
                let rectX = ptObject.person_bounding_box.x * scaleFactor;
                let rectY = ptObject.person_bounding_box.y * scaleFactor;
                let rectW = ptObject.person_bounding_box.w * scaleFactor;
                let rectH = ptObject.person_bounding_box.h * scaleFactor;

                if(ptObject.hasOwnProperty('rid')) {
                    ctx2d.strokeStyle = 'rgba(0, 238, 0, 1.0)';      // Color of pointing - green
                } else {
                    ctx2d.strokeStyle = 'rgba(250, 50, 50, 1.0)';      // Color of pointing - red
                }
                ctx2d.strokeRect(rectX, rectY, rectW, rectH); // Draw each person rectangle

                if(ptObject.hasOwnProperty('rid')) {
                    ctx2d.strokeStyle = 'rgba(0, 238, 0, 1.0)';      // Color of pointing - green
                    ctx2d.fillText('rid: ' + ptObject.rid, rectX+1, rectY+10);
                } else {
                    ctx2d.fillText('rid: ??', rectX+1, rectY+10);
                }
            }

            if(ptObject.hasOwnProperty('center_mass_image')) {
                // Draw center of mass image
                ctx2d.strokeRect(ptObject.center_mass_image.x * scaleFactor,
                                 ptObject.center_mass_image.y * scaleFactor, 1, 1);
            }

            // PT sample 2 result
            if(ptObject.hasOwnProperty('head_bounding_box')) {
                let rectX = ptObject.head_bounding_box.x * scaleFactor;
                let rectY = ptObject.head_bounding_box.y * scaleFactor;
                let rectW = ptObject.head_bounding_box.w * scaleFactor;
                let rectH = ptObject.head_bounding_box.h * scaleFactor;

                ctx2d.strokeRect(rectX, rectY, rectW, rectH); // Draw head bounding box
            }

            if(ptObject.hasOwnProperty('head_pose')) {
                let Pitch = ptObject.head_pose.pitch.toFixed(4);
                let Roll = ptObject.head_pose.roll.toFixed(4);
                let Yaw = ptObject.head_pose.yaw.toFixed(4);

                // To Reduce print overflow
                if(Math.abs(this.prevX-Pitch)>5.0 || Math.abs(this.prevY-Roll)>4.0 ||
                    Math.abs(this.prevZ-Yaw)>5.0) {
                    dataFinal += ';' + Pitch + ', ' + Roll + ', ' + Yaw;
                    this.prevX=Pitch;
                    this.prevY=Roll;
                    this.prevZ=Yaw;
                    headPoseResult = true;
                }
            }

            if(ptObject.hasOwnProperty('person_orientation')) {
                let Orientation = ptObject.person_orientation.orientation;
                let Confidence = ptObject.person_orientation.confidence;

                if(this.prevOrientation != Orientation) {
                    this.prevOrientation = Orientation;
                    orientationResult = true;
                }

                if(headPoseResult == true) {
                    dataFinal += ';' + Orientation + '(' + Confidence + '%)';
                } else {
                    dataFinal += ';' + 'Not Detected' + ';' + Orientation +
                                  '(' + Confidence + '%)';
                }
            }

            // PT sample 3 result
            if(ptObject.hasOwnProperty('gesture_color_coordinates')) {
                let OriginX = ptObject.gesture_color_coordinates.origin.x;
                let OriginY = ptObject.gesture_color_coordinates.origin.y;
                let DirectionX = ptObject.gesture_color_coordinates.direction.x;
                let DirectionY = ptObject.gesture_color_coordinates.direction.y;

                let FromX = OriginX * scaleFactor;
                let FromY = OriginY * scaleFactor;
                let ToX = (OriginX + 400 * DirectionX) * scaleFactor;
                let ToY = (OriginY + 400 * DirectionY) * scaleFactor;

                // console.log('Origin - Direction XY: ' + OriginX + ', ' + OriginY + ', ' +
                // DirectionX + ', ' + DirectionY);
                // console.log('From - t0 XY: ' + FromX + ', ' + FromY + ', ' + ToX + ', ' + ToY);
                let headlen = 10;   // length of head in pixels
                let angle = Math.atan2(ToY-FromY, ToX-FromX);

                ctx2d.strokeStyle = 'rgba(0, 238, 0, 1.0)';      // Color of pointing - green
                ctx2d.beginPath();
                ctx2d.moveTo(FromX, FromY);
                ctx2d.lineTo(ToX, ToY);
                ctx2d.lineTo(ToX-headlen*Math.cos(angle-Math.PI/6),
                             ToY-headlen*Math.sin(angle-Math.PI/6));
                ctx2d.moveTo(ToX, ToY);
                ctx2d.lineTo(ToX-headlen*Math.cos(angle+Math.PI/6),
                             ToY-headlen*Math.sin(angle+Math.PI/6));
                ctx2d.stroke();
            }

            if(ptObject.hasOwnProperty('gesture_world_coordinates')) {
                let WorldOriginX = ptObject.gesture_world_coordinates.origin.x.toFixed(4);
                let WorldOriginY = ptObject.gesture_world_coordinates.origin.y.toFixed(4);
                let WorldOriginZ = ptObject.gesture_world_coordinates.origin.z.toFixed(4);

                let WorldDirectionX = ptObject.gesture_world_coordinates.direction.x.toFixed(4);
                let WorldDirectionY = ptObject.gesture_world_coordinates.direction.y.toFixed(4);
                let WorldDirectionZ = ptObject.gesture_world_coordinates.direction.z.toFixed(4);

                // To Reduce print overflow
                if( Math.abs(this.prevX-WorldDirectionX)>0.05 ||
                    Math.abs(this.prevY-WorldDirectionY)>0.05 ||
                    Math.abs(this.prevZ-WorldDirectionZ)>0.05) {
                    this.prevX = WorldDirectionX;
                    this.prevY = WorldDirectionY;
                    this.prevZ = WorldDirectionZ;
                    gestureResult = true;
                }

                dataFinal += ';' + WorldOriginX + ', ' + WorldOriginY + ', ' + WorldOriginZ + ';' +
                              WorldDirectionX + ', ' + WorldDirectionY + ', ' + WorldDirectionZ;
            }

            if( (trackingResult == true) || (headPoseResult == true) ||
                (orientationResult == true) || (gestureResult == true))
                insertDataInTableRows(dataFinal);
        }
    }
};

function onOverlayCanvasClick(e) {
    console.log('< < < < < < < < < < < < < < < < < < < < ' +
                'onOverlayCanvasClick > > > > > > > > > > > > > > > > > >');
    let element = overlayCanvas;
    let offsetX = 0;
    let offsetY = 0;
    let scaleFactor = 2;

    if (element.offsetParent) {
        do {
            offsetX += element.offsetLeft;
            offsetY += element.offsetTop;
        } while ((element = element.offsetParent));
    }

    x = e.pageX - offsetX;
    y = e.pageY - offsetY;
    console.log('Computed x,y of click in canvas as ' + x + ', ' + y);
    // Next we need to check if the click position (x, y) is within on of the person_bounding_box
    // for any person we currently know about (by looking at the prevPtResultList)
    for (let i = 0; i < prevPtResultList.length; i++) {
        let ptObject = prevPtResultList[i];

        if (ptObject.hasOwnProperty('person_bounding_box')) {
            let rectX = ptObject.person_bounding_box.x * scaleFactor;
            let rectY = ptObject.person_bounding_box.y * scaleFactor;
            let rectW = ptObject.person_bounding_box.w * scaleFactor;
            let rectH = ptObject.person_bounding_box.h * scaleFactor;
            console.log('Person ' + i + ':  l, r, t, b = ' + rectX + ', ' + (rectX+rectW) + ',' +
                        rectY + ', ' + (rectY+rectH));

            // If click is in a person's rect
            if (x >= rectX && x < rectX+rectW && y >= rectY && y < rectY+rectH) {
                console.log('Click was in person ' + i);
                // and if we have ID for the person
                if (ptObject.hasOwnProperty('pid')) {
                    // Send message back through web socket to the C++ code saying to
                    // focus on this person
                    console.log('Clicked-on person has ID = ' + ptObject.pid);
                    transporter.sendMessage({type: 'pt_track', command: ptObject.pid});
                    return;	// Found clicked-on person, sent message back to server.  We're done.
                }
            }
        }
    }

    // The click didn't hit anybody, so send message back through web socket to the C++ code
    // saying to focus on nobody
    console.log('Clicked-on NO person, so telling server to stop focusing on all people');
    transporter.sendMessage({type: 'pt_track', command: '-1'});
}


transporter.onClearData = (timestamp) => {
    // deleteTableRow(); // Deteting the previously printed rows

    let canvas2d=document.getElementById('color-2d-overlay');
    if (canvas2d == null) {
        console.log('Failed to get color-2d-overlay element');
    }

    let ctx2d = canvas2d.getContext('2d');
    if (ctx2d == null) {
        console.log('Failed to get 2d context from color-2d-overlay element');
    }

    ctx2d.clearRect(0, 0, canvas2d.width, canvas2d.height);    // Clear the overlay canvas

    // let data = getCurrentTime(timestamp) + ', ' + 'Num people in frame: 0';
    // insertDataInTableRows(data);
};

transporter.onColorFrame = (width, height, data) => {
    rgbView.updateBuffer(data, width, height, THREE.RGBFormat);
    requestAnimationFrame(rgbRenderCall);
};

function insertDataInTableRows(data) {
    let entry = data.split(';');
    let table = document.getElementById('tBody');
    let currTableRows = table.rows.length;

    if(currTableRows === maxTableRows) {
        // console.log('Table has max rows. Deleting one row');
        lastRow = table.rows.length - 1;             // set the last row index

        table.deleteRow(lastRow);
    }
    let row = table.insertRow(0);
    let arrayLength = entry.length;
    for (let count = 0; count < arrayLength; count++) {
        fillCell(row.insertCell(count), entry[count], 'row');
    }
}

// create <div> element and append to the table cell
function fillCell(cell, text, style) {
    let div = document.createElement('div'); // create DIV element
    let txt = document.createTextNode(text); // create text node
    div.appendChild(txt);                    // append text node to the DIV
    div.setAttribute('class', style);        // set DIV class attribute
    div.setAttribute('className', style);    // set DIV class attribute for IE (?!)
    cell.appendChild(div);                   // append DIV to the table cell
}

function zeroPadding(time, numberOfZeros) {
    while (time.toString().length < numberOfZeros) {
        time = '0' + time;
    }
    return time;
}

function getCurrentTime(date) {
    let h = zeroPadding(date.getHours(), 2);
    let m = zeroPadding(date.getMinutes(), 2);
    let s = zeroPadding(date.getSeconds(), 2);
    let ms = zeroPadding(date.getMilliseconds(), 3);
    return(h + ':' + m + ':' + s + ':' + ms);
}

setTimeout(function() {
    transporter.open();
}, 100);
