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
        publishClick(radiusData.radius_dist);
    };

    const clear = () => {
        setradiusData({ radius_dist: 0});
    };

    const publishClick = (message) => {
      const client = mqtt.connect("ws://ec2-3-21-76-51.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
      client.on('connect', function () {
        console.log('Connected to broker');
        client.subscribe("radiusMode", error => {
          if (error) console.error(error);
          else {
            client.publish('radiusMode', message);
          }
        });
      });
      //client.end();
    }
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