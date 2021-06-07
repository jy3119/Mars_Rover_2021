import React, { useState } from 'react';
import { AppBar, Typography, Toolbar, Avatar, Button, Grid } from '@material-ui/core';
import HomeIcon from '@material-ui/icons/Home'; 
import { Link, useHistory, useLocation } from 'react-router-dom';
import useStyles from './styles'; 
import {Alert, AlertTitle} from '@material-ui/lab';
import mqtt from 'mqtt';

import BatteryContainer from './BatteryContainer';

const NavBar = () => {
    const classes = useStyles();

    const [control_mode, setcontrol_mode] = useState(false);

    const [battery, setBattery] = useState({ level: 0, charging: false }); 

    const [health, sethealth] = useState(null); 

    /*MQTT Subscribing*/
  var client = mqtt.connect("ws://ec2-3-21-102-39.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
  
  //setup the callbacks
  client.on('connect', function () {
    console.log('battery connected to broker');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  //dataString = String(SOC)+","+String(SOH)+","+String(state_num)
  
  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var SOC = Number(parse_coord[0]); 
    var SOH = Number(parse_coord[1]); 
    var state_num = parse_coord[2];
    if (state_num == '1') {var charge = true;}
    if (state_num == '0') {var charge = false;}
    /* include SOH later */ 
    setBattery({ level: SOC, charging: charge});
    sethealth(SOH); 
    console.log('Received message:', topic, message.toString());
});

  // subscribe to topic 'my/test/topic'
  client.subscribe('battery');


    return (
    <AppBar className={classes.appBar} position="static">
        <Typography variant="h2" align ="center" color="inherit">
        MARS ROVER
        </Typography>
        <Toolbar className={classes.toolbar}>
        {control_mode? (
          <div className={classes.profile}>
            <Typography className={classes.userName} variant="h6">{control_mode}</Typography>
            {(health<20) ? 
              <Alert severity="error" fontSize='10'>Change battery</Alert> 
              :
              <Alert severity="info" className={classes.notifs} display='flex'>
              Battery SOH: {health} 
              </Alert>
            }
            <BatteryContainer {...battery}/>
            <Button component={Link} to="/" onClick={()=>{setcontrol_mode(false)}}> 
                <HomeIcon variant="contained" color="secondary">Home</HomeIcon>
            </Button>
          </div>
        ) : (
          <div className={classes.profile}> 
                {(health<20) ? 
                  <Alert severity="error" className={classes.soh_alert}>Change battery</Alert> 
                  :
                  <Alert severity="info" className={classes.soh_alert} display='flex'>
                  Battery SOH: {health} 
                  </Alert>
                }
                <BatteryContainer {...battery}/>
                <Button className={classes.button_cmode} component={Link} to="/coord_mode" variant="contained" color="secondary" onClick={()=>setcontrol_mode(true)}>Automatic Mode</Button>
                <Button className={classes.button_dmode} component={Link} to="/direct_mode" variant="contained" color="secondary" onClick={()=>setcontrol_mode(true)}>Manual Mode</Button>
          </div>
        )}
      </Toolbar>
    </AppBar>
    );
};

export default NavBar;

