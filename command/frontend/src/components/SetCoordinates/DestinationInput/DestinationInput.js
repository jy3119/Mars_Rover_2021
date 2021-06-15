import React, { useState } from 'react';
import { TextField, Button, Typography, Paper, Container, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux';

/* MQTT Setup */
import mqtt from 'mqtt';

import useStyles from './styles';
import { createDestination } from '../../../actions/destinations-actions';


const DestinationInput = () => {
  const [destinationData, setdestinationData] = useState({
      x_coordinate: 0, y_coordinate: 0, radius_dist: 0
  }); 

  const [id, setid] = useState(-1);

  const dispatch = useDispatch();
  const classes = useStyles();

  const handleSubmit = async (e) => {
      e.preventDefault();
      
      dispatch(createDestination(destinationData));
      clear();
      var mes = (id.toString()).concat(',',destinationData.x_coordinate,',',destinationData.y_coordinate,',',destinationData.radius_dist);
      publishClick(mes);
    };

  const clear = () => {
      setdestinationData({ x_coordinate: 0, y_coordinate: 0, radius_dist: 0});
    };

  /*MQTT Publishing*/
  const publishClick = (message) => {
      /*
      var options = {
      port: 8083,
      username: 'mqtt-websockets',
      password: 'coolbeans1234',
      clean: true,
      useSSL: true,
      ca: fs.readFileSync('../../../../mqtt-js-aws/ca.crt')
      }*/
  
      //initialize the MQTT client
      //var client = mqtt.connect("wss://ec2-18-216-115-209.us-east-2.compute.amazonaws.com/mqtt", options);
    const client = mqtt.connect("ws://ec2-18-223-15-156.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
    client.on('connect', function () {
      console.log('Connected to broker');
      client.subscribe("auto", error => {
        if (error) console.error(error);
        else {
          client.publish('auto', message);
        }
      });
    });
    //client.end();
  }


  return (
  <Container>
    <Grid>
    <Paper className={classes.paper} elevation={6}>
      <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
        <Typography variant="h5">Setting Coordinates</Typography>
        <TextField name="x-coordinate" variant="outlined" label="x-coordinate" fullWidth value={destinationData.x_coordinate} onChange={(e) => setdestinationData({ ...destinationData, x_coordinate: e.target.value })} />
        <TextField name="y-coordinate" variant="outlined" label="y-coordinate" fullWidth value={destinationData.y_coordinate} onChange={(e) => setdestinationData({ ...destinationData, y_coordinate: e.target.value })} />
        <TextField name="radius" variant="outlined" label="radius" fullWidth value={destinationData.radius_dist} onChange={(e) => setdestinationData({ ...destinationData, radius_dist: e.target.value })} />
        <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth onClick={() => setid(id+1)}>Submit</Button>
        <Button variant="contained" color="secondary" size="small" onClick={clear} fullWidth>Clear</Button>
      </form>
    </Paper>
    </Grid>
  </Container>
  );
};

 
export default DestinationInput;

