import React, { Component } from 'react';
import TextField from '@material-ui/core/TextField';
import '@fontsource/roboto';
import Typography from '@material-ui/core/Typography';

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