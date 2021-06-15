import React, {useState} from 'react';
import {Alert, AlertTitle} from '@material-ui/lab';

import mqtt from 'mqtt';

const RoverPos = () => {
    const [positionData, setpositionData] = useState({
        x_coord: 0, y_coord: 0, angle: 0
    }); 
    
    var client = mqtt.connect("ws://ec2-18-223-15-156.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});

    //setup the callbacks
  client.on('connect', function () {
    console.log('RoverPos connected to broker');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var rover_x = Number(parse_coord[0]); 
    var rover_y = Number(parse_coord[1]); 
    var steer_angle = Number(parse_coord[2]);
    setpositionData({x_coord: rover_x, y_coord: rover_y, angle: steer_angle});
    console.log('Received message:', topic, message.toString());
});

  client.subscribe('liveloc');

  return (
    <Alert severity="info">
        rover is at position ({positionData.x_coord},{positionData.y_coord}) at orientation {positionData.angle} degrees
    </Alert>
  )
}

export default RoverPos; 