import React, { Component, useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';

const DestinationInput = () => {
    const [destinationData, setdestinationData] = useState({
        xcoord: 0, ycoord: 0
    });
    const dispatch = useDispatch();
    const classes = useStyles();
  
    /*
    useEffect(() => {
      if (post) setPostData(post);
    }, [post]);*/
  
    const handleSubmit = () => {
    };

    const clear = () => {
        setdestinationData({ xcoord: 0, ycoord: 0});
      };
  
    return (
      <Paper className={classes.paper} position="absolute" >
        <form autoComplete="off" noValidate className={`${classes.root} ${classes.form}`} onSubmit={handleSubmit}>
          <Typography variant="h5">Destination</Typography>
          <TextField name="x-coordinate" variant="outlined" label="x-coordinate" fullWidth value={destinationData.xcoord} onChange={(e) => setdestinationData({ ...destinationData, xcoord: e.target.value })} />
          <TextField name="y-coordinate" variant="outlined" label="y-coordinate" fullWidth value={destinationData.ycoord} onChange={(e) => setdestinationData({ ...destinationData, ycoord: e.target.value })} />
          <Button className={classes.buttonSubmit} variant="contained" color="primary" size="large" type="submit" fullWidth>Submit</Button>
          <Button variant="contained" color="secondary" size="small" onClick={clear} fullWidth>Clear</Button>
        </form>
      </Paper>
    );
  };
  
 
export default DestinationInput;