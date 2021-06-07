import React, {useState} from 'react';
import {Alert, AlertTitle} from '@material-ui/lab';
import { Collapse, IconButton } from '@material-ui/core';
import CloseIcon from '@material-ui/icons/Close';
import moment from 'moment';
import { useDispatch } from 'react-redux';

import mqtt from 'mqtt';

import useStyles from './styles';
import { deleteInstruction } from '../../../actions/instructions-actions';

/* Brief 
  Subscribe to topic warnings
  if new message received, then show alert? 
*/ 

const Warning = () => {
  const dispatch = useDispatch();
  const classes = useStyles();
  const [warningData, setwarningData] = useState({
    x_coord: 0, y_coord: 0, colour: ''
  }); 

  //some sort of boolean that shows its the first warning or smthng 

  const [positionData, setpositionData] = useState({
    x_coord: 0, y_coord: 0
  }); 

  const [open, setOpen] = useState(true);

  /*MQTT Subscribing*/
  var client = mqtt.connect("ws://ec2-3-21-102-39.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
  
  //setup the callbacks
  client.on('connect', function () {
    console.log('warning connected to broker');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var x = Number(parse_coord[0]); 
    var y = Number(parse_coord[1]); 
    var colour_num = parse_coord[2];
    if (topic == 'obstacle') {
       /* add in corresponding colour to the numbers */ 
       if (colour_num=='0') {var obs_colour = 'red';}
       if (colour_num=='1') {var obs_colour = 'green';}
       if (colour_num=='2') {var obs_colour = 'blue';}
       if (colour_num=='3') {var obs_colour = 'pink';}
       if (colour_num=='4') {var obs_colour = 'yellow';}
      setwarningData({x_coord: x, y_coord: y, colour: obs_colour});
    }
    else if (topic == 'liveloc') {
      setpositionData({x_coord: x, y_coord: y});
    }
    console.log('Received message:', topic, message.toString());
});

  // subscribe to topic 'my/test/topic'
  client.subscribe('obstacle');

  client.subscribe('liveloc');
   
  return (
    (warningData.x_coord==0 && warningData.y_coord==0) ? 
    <div className={classes.root}>
      <Alert severity="info">
        rover is at position ({positionData.x_coord},{positionData.y_coord})
      </Alert>
    </div> :
    <div className={classes.root}>
    <Collapse in={open}>
      <Alert
        severity="warning"
        className={classes.notifs}
        action={
          <IconButton
            aria-label="close"
            color="inherit"
            size="small"
            onClick={() => {
              setOpen(false);
            }}
          >
            <CloseIcon fontSize="inherit" />
          </IconButton>
        }
      >
        <AlertTitle>Warning</AlertTitle>
        rover stopped before {warningData.colour} obstacle at ({warningData.x_coord},{warningData.y_coord}) — check it out! 
      </Alert>
      </Collapse>
      <Alert severity="info" className={classes.notifs}>
        rover is at position ({positionData.x_coord},{positionData.y_coord})
      </Alert>
  </div>
      
  );
};
  
export default Warning;
