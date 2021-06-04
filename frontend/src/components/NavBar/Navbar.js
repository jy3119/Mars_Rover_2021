import React, { useState } from 'react';
import { AppBar, Typography, Toolbar, Avatar, Button, Grid } from '@material-ui/core';
import HomeIcon from '@material-ui/icons/Home'; 
import { Link, useHistory, useLocation } from 'react-router-dom';
import useStyles from './styles'; 

const NavBar = () => {
    const classes = useStyles();

    const [control_mode, setcontrol_mode] = useState(false);

    return (
    <AppBar className={classes.appBar} position="static">
        <Typography variant="h2" align ="center" color="inherit">
        MARS ROVER
        </Typography>
        <Toolbar className={classes.toolbar}>
        {control_mode? (
          <div className={classes.profile}>
            <Typography className={classes.userName} variant="h6">{control_mode}</Typography>
            <Button component={Link} to="/" onClick={()=>{setcontrol_mode(false)}}> 
                <HomeIcon variant="contained" color="secondary">Home</HomeIcon>
            </Button>
          </div>
        ) : (
          <div> 
                <Button className={classes.button_cmode} component={Link} to="/coord_mode" variant="contained" color="secondary" onClick={()=>setcontrol_mode(true)}>Automatic Mode</Button>
                <Button className={classes.button_dmode} component={Link} to="/direct_mode" variant="contained" color="secondary" onClick={()=>setcontrol_mode(true)}>Manual Mode</Button>
          </div>
        )}
      </Toolbar>
    </AppBar>
    );
};

export default NavBar;

