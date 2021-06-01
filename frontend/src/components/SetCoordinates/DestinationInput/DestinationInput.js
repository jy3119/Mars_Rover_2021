import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper, Container, Grid } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

/* MQTT Setup */
import mqtt, { MqttClient } from 'mqtt';
import { Connector,  useMqttState, publish } from 'mqtt-react-hooks';

import useStyles from './styles';
import { createDestination, updateDestination } from '../../../actions/destinations-actions';

const DestinationInput = ({ currentId, setCurrentId }) => {
  const [destinationData, setdestinationData] = useState({
      x_coordinate: 0, y_coordinate: 0
  }); 
  //find post where id is equal to current id 
  const destination = useSelector((state) => (currentId ? state.destinations.find((message) => message._id === currentId) : null));
  const dispatch = useDispatch();
  const classes = useStyles();
  
  useEffect(() => {
      if (destination) setdestinationData(destination);
  }, [destination]); 

  const handleSubmit = async (e) => {
      e.preventDefault();

      if (currentId === 0) {
        dispatch(createDestination(destinationData));
        clear();
      } else {
        dispatch(updateDestination(currentId, destinationData));
        clear();
      }
    };

  const clear = () => {
      setCurrentId(null);
      setdestinationData({ x_coordinate: 0, y_coordinate: 0});
    };
  
  //MQTT Publishing
  //const [ConnectionStatus, setConnectionStatus] = useState(false);
  //const { client } = useMqttState();

  /*useEffect(() => {
    var client = mqtt.connect("mqtt://ec2-18-223-237-159.us-east-2.compute.amazonaws.com", {port:1883});
    client.on('connect', () => setConnectionStatus(true));
    var options = {qos: 1};
    client.publish("my/test/topic", destinationData, options);
  }, []);*/

  var myMqtt = new MqttClient();
  var client = myMqtt.connect("mqtt://ec2-18-191-210-250.us-east-2.compute.amazonaws.com", {port:1883});
  client.on('connect', function () {
    console.log('Connected');
  });
  const publishClick = (message) => {
    client.publish('my/test/topic', message);
  }

  return (
  <Container>
    <Grid>
    <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:200}}>
      <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
        <Typography variant="h5">{ currentId ? 'Editing' : 'Setting'} Coordinates</Typography>
        <TextField name="x-coordinate" variant="outlined" label="x-coordinate" fullWidth value={destinationData.x_coordinate} onChange={(e) => setdestinationData({ ...destinationData, x_coordinate: e.target.value })} />
        <TextField name="y-coordinate" variant="outlined" label="y-coordinate" fullWidth value={destinationData.y_coordinate} onChange={(e) => setdestinationData({ ...destinationData, y_coordinate: e.target.value })} />
        <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth onClick={publishClick('hello world')}>Submit</Button>
        <Button variant="contained" color="secondary" size="small" onClick={clear} fullWidth>Clear</Button>
      </form>
    </Paper>
    </Grid>
    <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth onClick={publishClick('hello world')}>Submit</Button>
  </Container>
  );
};

 
export default DestinationInput;