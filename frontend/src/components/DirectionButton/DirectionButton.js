import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';

import useStyles from './styles';
//import { createInstruction } from '../../actions/instructions'; 

const DirectionButton = () => {
    const [instructionData, setinstructionData] = useState({
        direction: '', distance: 0, angle: 0, speed: 0
    }); 

    const classes = useStyles();

    const handleSubmit = async (e) => {
    
    };

    const clear = () => {

    };

    return (
      <Paper className={classes.paper} style={{width: 300, position: 'absolute', right:200, top:100}}>
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">Setting Direction</Typography>
          <TextField name="distance" variant="outlined" label="distance" fullWidth value={instructionData.distance} onChange={(e) => setinstructionData({ ...instructionData, distance: e.target.value })} />
          <Button className={classes.direction_button} variant="contained" color="primary">Forward</Button>
          <Button className={classes.direction_button} variant="contained" color="primary">Back</Button>
          <TextField name="angle" variant="outlined" label="angle" fullWidth value={instructionData.angle} onChange={(e) => setinstructionData({ ...instructionData, angle: e.target.value })} />
          <Button className={classes.direction_button} variant="contained" color="primary">Left</Button>
          <Button className={classes.direction_button} variant="contained" color="primary">Right</Button>
          <TextField name="speed" variant="outlined" label="speed" fullWidth value={instructionData.speed} onChange={(e) => setinstructionData({ ...instructionData, speed: e.target.value })} />
          <Button className={classes.full_width_button} variant="contained" color="secondary" fullWidth>STOP</Button>
          <Button className={classes.full_width_button} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
          <Button variant="contained" color="textSecondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };

//style={{width: 100,position: 'absolute', right:320, top:150}}

export default DirectionButton;