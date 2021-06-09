import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper, MenuItem, Menu } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { createInstruction } from '../../../actions/instructions-actions'; 

/* MQTT Setup */
import mqtt from 'mqtt';

const directions = [
  {
    value: 'Forward',
    label: 'Forward',
  },
  {
    value: 'Back',
    label: 'Back',
  },
];


const DirectionButton = () => {
    const [instructionData, setinstructionData] = useState({
        direction: 'Forward', distance: 5, angle:0, speed: 5
    }); 

    const [id, setid] = useState(-1);

    const classes = useStyles();
    const dispatch = useDispatch();

    const handleSubmit = async (e) => {
        e.preventDefault();
        dispatch(createInstruction(instructionData));
        //0 for Forward, 1 for Backward
        var dir_int;
        if (instructionData.direction=='Forward') {dir_int='0';}
        else {dir_int='1';}
        var mes = (id.toString()).concat(',',dir_int,',',instructionData.distance,',',instructionData.angle,',',instructionData.speed);
        publishClick(mes);
    };

    const clear = () => {
        setinstructionData({ direction: 'Forward', distance: 5, angle: 0, speed: 5});
    };

    const stop = () => {
        setinstructionData({ direction: 'Forward', distance: 0, angle: 0, speed: 0});
    };

     /*MQTT Publishing*/
     const publishClick = (message) => {
      const client = mqtt.connect("ws://ec2-18-223-15-156.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
      client.on('connect', function () {
        console.log('form connected to broker');
        client.subscribe("manual", error => {
          if (error) console.error(error);
          else {
            client.publish('manual', message);
          }
        });
      });
      //client.end();
    }


  const handleChange = (e) => {
    setinstructionData({ ...instructionData, direction: e.target.value })
  };

    return (
      <Paper className={classes.paper} elevation={6}>
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">Setting Instruction</Typography>
          <TextField
          id="direction"
          select
          label="direction"
          value={instructionData.direction}
          onChange={handleChange}
          variant="outlined"
          fullWidth
        >
          {directions.map((option) => (
            <MenuItem key={option.value} value={option.value}>
              {option.label}
            </MenuItem>
          ))}
        </TextField>
          <TextField name="distance" variant="outlined" label="distance" fullWidth value={instructionData.distance} onChange={(e) => setinstructionData({ ...instructionData, distance: e.target.value })} />
          <TextField name="angle" variant="outlined" label="angle" fullWidth value={instructionData.angle} onChange={(e) => setinstructionData({ ...instructionData, angle: e.target.value })} helperText="Please select between -180 to 180"/>
          <TextField name="speed" variant="outlined" label="speed" fullWidth value={instructionData.speed} onChange={(e) => setinstructionData({ ...instructionData, speed: e.target.value })} />
          <Button className={classes.full_width_button} variant="contained" color="secondary" fullWidth onClick={stop}>STOP</Button>
          <Button className={classes.full_width_button} variant="contained" color="primary" size="large" type="submit" fullWidth onClick={() => setid(id+1)}>Submit</Button>
          <Button className={classes.full_width_button} variant="contained" color="textSecondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };

//style={{width: 100,position: 'absolute', right:320, top:150}}
//<TextField name="direction" variant="outlined" label="direction" fullWidth id="direction" /> 
/*
<Button className={classes.direction_button} variant="contained" color="primary" id="Forward" onClick={()=>setinstructionData({ ...instructionData, direction: "Forward" })}>Forward</Button>
<Button className={classes.direction_button} variant="contained" color="primary" id="Back" onClick={()=>setinstructionData({ ...instructionData, direction: "Back" })}>Back</Button>
*/

export default DirectionButton;