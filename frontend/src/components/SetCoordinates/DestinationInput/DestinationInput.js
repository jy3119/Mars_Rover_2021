import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper, Container, Grid } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

/* MQTT Setup */
import mqtt, { MqttClient } from 'mqtt';
import { Connector,  useMqttState, publish } from 'mqtt-react-hooks';

import useStyles from './styles';
import { createDestination } from '../../../actions/destinations-actions';


const DestinationInput = () => {
  const [destinationData, setdestinationData] = useState({
      x_coordinate: 0, y_coordinate: 0
  }); 
  const dispatch = useDispatch();
  const classes = useStyles();

  const handleSubmit = async (e) => {
      e.preventDefault();
      
      dispatch(createDestination(destinationData));
      clear();
      var mes = (destinationData.x_coordinate).concat(',',destinationData.y_coordinate);
      publishClick(mes);
    };

  const clear = () => {
      setdestinationData({ x_coordinate: 0, y_coordinate: 0});
    };

  /*MQTT Publishing*/
  const publishClick = (message) => {
    const client = mqtt.connect("ws://ec2-18-224-199-255.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
    client.on('connect', function () {
      console.log('Connected to broker');
      client.subscribe("coordsMode", error => {
        if (error) console.error(error);
        else {
          client.publish('coordsMode', message);
        }
      });
    });
    //client.end();
  }


  return (
  <Container>
    <Grid>
    <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:200}}>
      <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
        <Typography variant="h5">Setting Coordinates</Typography>
        <TextField name="x-coordinate" variant="outlined" label="x-coordinate" fullWidth value={destinationData.x_coordinate} onChange={(e) => setdestinationData({ ...destinationData, x_coordinate: e.target.value })} />
        <TextField name="y-coordinate" variant="outlined" label="y-coordinate" fullWidth value={destinationData.y_coordinate} onChange={(e) => setdestinationData({ ...destinationData, y_coordinate: e.target.value })} />
        <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
        <Button variant="contained" color="secondary" size="small" onClick={clear} fullWidth>Clear</Button>
      </form>
    </Paper>
    </Grid>
  </Container>
  );
};

 
export default DestinationInput;

/*
export default () => {
  const [connectionStatus, setConnectionStatus] = React.useState(false);
  const [messages, setMessages] = React.useState([]);
 
  useEffect(() => {
    const client = mqtt.connect(SOME_URL);
    client.on('connect', () => setConnectionStatus(true));
    client.on('message', (topic, payload, packet) => {
      setMessages(messages.concat(payload.toString()));
    });
  }, []);

  function CheckoutForm() {
    const MQTTConnect = () => {
        const client = mqtt.connect("mqtt://address.cloudmqtt.com", options);
        client.on("connect", function() {
          // When connected
          console.log("connected");
          client.subscribe("vendingmachine2/feedback", error => {
            if (error) console.error(error);
            else {
              client.publish(topic, "0");
            }
          });
          openDoor();
        });
        client.on("message", (topic, message) => {
          console.log(topic, message.toString());
        });
        function openDoor() {
          let door = [1, 2];
          for (let i = 0; i < door.length; i++) {
            client.publish(topic, `${door[i]}`);
          }
        }
    };


  */