import React, { Component } from 'react';
import Button from '@material-ui/core/Button'
import { createStyles, makeStyles, Theme } from '@material-ui/core/styles'
//import { createInstruction, updateInstruction } from '../../actions/instructions'; 

class DirectionButton extends Component {
    render() { 
        return ( 
            <div>
                <Button variant="contained" color="primary" style={{width: 100, position: 'absolute', right:200, top:100}}>Forward</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:200, top:200}}>Back</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:320, top:150}}>Left</Button>
                <Button variant="contained" color="primary" style={{width: 100,position: 'absolute', right:80, top:150}}>Right</Button>
                <Button variant="contained" color="secondary" style={{width: 100,position: 'absolute', right:200, top:150}}>STOP</Button>
            </div>
         );
    }
}
 
export default DirectionButton;