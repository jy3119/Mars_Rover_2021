import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';

import useStyles from './styles';
//import { createInstruction } from '../../actions/instructions'; 

const DirectionButton = () => {
    const classes = useStyles();

    return (
      <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:100}}>
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">{ currentId ? 'Editing' : 'Setting'} Destination</Typography>
          <TextField name="x-coordinate" variant="outlined" label="x-coordinate" fullWidth value={destinationData.x_coordinate} onChange={(e) => {}} />
          <TextField name="y-coordinate" variant="outlined" label="y-coordinate" fullWidth value={destinationData.y_coordinate} onChange={(e) => {}} />
          <Button variant="contained" color="primary" style={{width: 100, position: 'absolute', right:200, top:100}}>Forward</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:200, top:200}}>Back</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:320, top:150}}>Left</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:80, top:150}}>Right</Button>
                <Button variant="contained" color="secondary" style={{width: 100,position: 'absolute', right:200, top:150}}>STOP</Button>
          <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
          <Button variant="contained" color="secondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };


export default DirectionButton;