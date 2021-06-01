import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { createRadius } from '../../../actions/radiuses-actions'; 

/* MQTT Setup */
import mqtt from 'mqtt';

const RadiusInput = () => {
    const [radiusData, setradiusData] = useState({
        radius_dist: 0,
    }); 

    const classes = useStyles();
    const dispatch = useDispatch();

    const handleSubmit = async (e) => {
        e.preventDefault();
        dispatch(createRadius(radiusData));
    };

    const clear = () => {
        setradiusData({ radius_dist: 0});
    };

     //MQTT Publishing
    const [ConnectionStatus, setConnectionStatus] = useState(false);

    useEffect(() => {
      var client = mqtt.connect("ws://18.188.43.23:9001", {
        //open connection with your broker in AWS via websocket
        username:"mqtt-broker", //authenticate your broker with username and password
        password:"coolbeans$4",
      });  //connecting the mqtt server with the MongoDB database
      client.on('connect', () => setConnectionStatus(true));
      var options = {qos: 1};
      client.publish("device/esp32/espradiuss", radiusData, options)
    }, []);

    return (
      <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:150}}>
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">Setting Radius</Typography>
          <TextField name="radius" variant="outlined" label="radius" fullWidth value={radiusData.radius_dist} onChange={(e) => setradiusData({ ...radiusData, radius_dist: e.target.value })} />
          <Button className={classes.full_width_button} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
          <Button variant="contained" color="textSecondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };

export default RadiusInput;