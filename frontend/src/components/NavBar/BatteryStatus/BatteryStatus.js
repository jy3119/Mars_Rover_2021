import React, {useState} from 'react';
import mqtt from 'mqtt';
import { Container } from '@material-ui/core'; 
import useStyles from './styles'; 

import BatteryContainer from './BatteryContainer/BatteryContainer';

/* Brief 
    Energy will send to local computer using UART 
    From local computer, publish to MQTT Broker to be subscribed to from the web browser 
*/

const BatteryStatus = () => {
    const [batteryStrokes, setBatteryStrokes] = useState(Array(5).fill(true));
    const classes = useStyles();

    const [battery, setBattery] = useState({ level: 0, charging: false }); 

    /*MQTT Subscribing*/
  var client = mqtt.connect("ws://ec2-3-21-102-39.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
  
  //setup the callbacks
  client.on('connect', function () {
    console.log('battery connected to broker');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  //dataString = .........+String(SOC)+","+String(SOH)
  
  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var SOC = Number(parse_coord[0]); 
    var SOH = Number(parse_coord[1]); 
    var state_num = parse_coord[2];
    if (state_num == '1') {var charge = true;}
    if (state_num == '0') {var charge = false;}
    /* include SOH later */ 
    setBattery({ level: SOC, charging: charge});
    console.log('Received message:', topic, message.toString());
});

  // subscribe to topic 'my/test/topic'
  client.subscribe('battery');

    return (
        <Container maxWidth='xs'>
            <BatteryContainer className={classes.battery} {...battery}/>
        </Container>
    );
}

export default BatteryStatus;