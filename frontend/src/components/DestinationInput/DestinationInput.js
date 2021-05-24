import React, { useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { createDestination } from '../../actions/destinations-actions';

const DestinationInput = () => {
    const [destinationData, setdestinationData] = useState({
        xcoord: '', ycoord: ''
    });
    //const destination = useSelector((state) => (currentId ? state.destinations.find((message) => message._id === currentId) : null));
    const dispatch = useDispatch();
    const classes = useStyles();

    
    /*useEffect(() => {
        if (destination) setdestinationData(destination);
    }, [destination]); */
  
    const handleSubmit = async (e) => {
        e.preventDefault();
        
        dispatch(createDestination(destinationData));
      };

    const clear = () => {
        setdestinationData({ xcoord: '', ycoord: ''});
      };
    
  
    return (
      <Paper className={classes.paper} style={{width: 300, position: 'absolute', left:200, top:100}}>
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