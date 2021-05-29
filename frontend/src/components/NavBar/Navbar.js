import React from 'react';
import { AppBar, Typography, Toolbar, Avatar, Button } from '@material-ui/core';
import { Link, useHistory, useLocation } from 'react-router-dom';
import useStyles from './styles'; 

const NavBar = () => {
    const classes = useStyles();

    const control_mode = null;

    return (
    <AppBar className={classes.appBar} position="static">
        <Typography variant="h2" align ="center" color="inherit">
        MARS ROVER
        </Typography>
        <Toolbar className={classes.toolbar}>
        {control_mode? (
          <div className={classes.profile}>
            <Typography className={classes.userName} variant="h6">{control_mode?.result.name}</Typography>
            <Button variant="contained" className={classes.logout} color="secondary" onClick={()=>{}}>Set Coordinates Mode</Button>
            <Button variant="contained" className={classes.logout} color="secondary" onClick={()=>{}}>Set Directions Mode</Button>
          </div>
        ) : (
          <Button component={Link} to="/auth" variant="contained" color="primary">Sign In</Button>
        )}
      </Toolbar>
    </AppBar>
    );
};

export default NavBar;