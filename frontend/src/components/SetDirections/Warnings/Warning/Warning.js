import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import Alert from '@material-ui/lab/Alert';
import moment from 'moment';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { deleteInstruction } from '../../../../actions/instructions-actions';

/* Brief 
  Subscribe to topic warnings
  if new message received, then show alert? 
*/ 

const Warning = ({instruction}) => {
  const dispatch = useDispatch();
  const classes = useStyles();

  /*MQTT Publishing*/
  const publishClick = (message) => {
    const client = mqtt.connect("ws://ec2-18-224-199-255.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
    client.on('connect', function () {
      console.log('Connected to broker');
      client.subscribe("manual", error => {
        if (error) console.error(error);
        else {
          client.publish('manual', message);
        }
      });
    });
    //client.end();
  }
   
  return (
      <div>
         <Alert variant="outlined" severity="warning">
          This is a warning alert — check it out!
        </Alert>
      </div>
  );
};
  
  
export default Warning;