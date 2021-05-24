import React, { Component, useState, useEffect } from 'react';
import { TextField, Button, Typography, Paper } from '@material-ui/core';
import { useDispatch, useSelector } from 'react-redux';
import useStyles from './styles';

class DestinationInput extends Component {
    render() { 
        return ( 
            <div>
                <Typography variant="h5" style={{position: 'absolute', right:910, top:120}} >
                    Destination:
                </Typography>
                <TextField
                    label = "y-coordinate"
                    variant = "outlined"
                    style={{position: 'absolute', right:500, top:100}}
                >
                </TextField>
                <TextField
                    label = "x-coordinate"
                    variant = "outlined"
                    style={{position: 'absolute', right:700, top:100}}
                >
                </TextField>
            </div>
         );
    }
}
 
export default DestinationInput;