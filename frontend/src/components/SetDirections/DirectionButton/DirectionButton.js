import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { createInstruction } from '../../../actions/instructions-actions'; 

/* MQTT Setup */
import mqtt from 'mqtt';

const DirectionButton = () => {
    const [instructionData, setinstructionData] = useState({
        direction: '', distance: 0, angle: 0, speed: 5
    }); 

    const classes = useStyles();
    const dispatch = useDispatch();

    const handleSubmit = async (e) => {
        e.preventDefault();
        dispatch(createInstruction(instructionData));
        var mes = (instructionData.distance).concat(',',instructionData.angle,',',instructionData.speed);
        publishClick(mes);
    };

    const clear = () => {
        setinstructionData({ direction: '', distance: 0, angle: 0, speed: 5});
    };

    const stop = () => {
        setinstructionData({ direction: '', distance: 0, angle: 0, speed: 0});
    };

    
    const fill_direction = async (id) => {
        //setinstructionData({ ...instructionData, distance: update })
        /*
        if (id=="Forward") {
            document.getElementById("direction").value = "Forward";
        }
        else if (id=="Back") {
             document.getElementById('direction').value = "Back";
        }
        else if (id=="Left") {
             document.getElementById('direction').value = "Left";
        }
        else if (id=="Right") {
             document.getElementById('direction').value = "Right";
        }
        else {
            document.getElementById('direction').value = ""
        }*/

     };

     /*MQTT Publishing*/
     const publishClick = (message) => {
      const client = mqtt.connect("ws://ec2-3-21-76-51.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
      client.on('connect', function () {
        console.log('Connected to broker');
        client.subscribe("instructionsMode", error => {
          if (error) console.error(error);
          else {
            client.publish('instructionsMode', message);
          }
        });
      });
      //client.end();
    }

    return (
      <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:150}}>
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">Setting Direction</Typography>
          <TextField name="distance" variant="outlined" label="distance" fullWidth value={instructionData.distance} onChange={(e) => setinstructionData({ ...instructionData, distance: e.target.value })} />
          <Button className={classes.direction_button} variant="contained" color="primary" id="Forward" onClick={()=>setinstructionData({ ...instructionData, direction: "Forward" })}>Forward</Button>
          <Button className={classes.direction_button} variant="contained" color="primary" id="Back" onClick={()=>setinstructionData({ ...instructionData, direction: "Back" })}>Back</Button>
          <TextField name="angle" variant="outlined" label="angle" fullWidth value={instructionData.angle} onChange={(e) => setinstructionData({ ...instructionData, angle: e.target.value })} />
          <Button className={classes.direction_button} variant="contained" color="primary" id="Left" onClick={()=>setinstructionData({ ...instructionData, direction: "Left" })}>Left</Button>
          <Button className={classes.direction_button} variant="contained" color="primary" id="Right" onClick={()=>setinstructionData({ ...instructionData, direction: "Right" })}>Right</Button>
          <TextField name="speed" variant="outlined" label="speed" fullWidth value={instructionData.speed} onChange={(e) => setinstructionData({ ...instructionData, speed: e.target.value })} />
          <Button className={classes.full_width_button} variant="contained" color="secondary" fullWidth onClick={stop}>STOP</Button>
          <Button className={classes.full_width_button} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
          <Button variant="contained" color="textSecondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };

//style={{width: 100,position: 'absolute', right:320, top:150}}
//<TextField name="direction" variant="outlined" label="direction" fullWidth id="direction" /> 

export default DirectionButton;