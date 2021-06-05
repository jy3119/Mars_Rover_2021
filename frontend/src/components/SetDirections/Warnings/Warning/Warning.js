import React, {useState} from 'react';
import Alert from '@material-ui/lab/Alert';
import { Collapse, IconButton } from '@material-ui/core';
import CloseIcon from '@material-ui/icons/Close';
import moment from 'moment';
import { useDispatch } from 'react-redux';

import mqtt from 'mqtt';

import useStyles from './styles';
import { deleteInstruction } from '../../../../actions/instructions-actions';

/* Brief 
  Subscribe to topic warnings
  if new message received, then show alert? 
*/ 

const Warning = () => {
  const dispatch = useDispatch();
  const classes = useStyles();
  const [warningData, setwarningData] = useState({
    x_coord: 0, y_coord: 0
  }); 

  const [open, setOpen] = useState(true);

  /*MQTT Subscribing*/
  var client = mqtt.connect("ws://ec2-3-131-135-32.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
  
  //setup the callbacks
  client.on('connect', function () {
    console.log('Connected');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var obstacle_x = Number(parse_coord[0]); 
    var obstacle_y = Number(parse_coord[1]); 
    setwarningData({x_coord: obstacle_x, y_coord: obstacle_y});
    console.log('Received message:', topic, message.toString());
});

  // subscribe to topic 'my/test/topic'
  client.subscribe('warning');
   
  return (
    <div className={classes.root}>
    <Collapse in={open}>
      <Alert
        variant="outlined" severity="warning"
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
        warning: rover stopped before obstacle at ({warningData.x_coord},{warningData.y_coord}) — check it out! 
      </Alert>
    </Collapse>
  </div>
      
  );
};
  
  
export default Warning;
