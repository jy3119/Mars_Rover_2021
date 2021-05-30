import React from 'react';
import { AppBar, Typography, Toolbar, Avatar, Button } from '@material-ui/core';
import HomeIcon from '@material-ui/icons/Home'; 
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
            <HomeIcon variant="contained" color="secondary" onClick={()=>{}}>Home</HomeIcon>
          </div>
        ) : (
          <div> 
              <Button component={Link} to="/coord_mode" variant="contained" color="secondary">Set Coordinates Mode</Button>
              <Button component={Link} to="/direct_mode" variant="contained" color="primary">Set Direction Mode</Button>
          </div>
        )}
      </Toolbar>
    </AppBar>
    );
};

export default NavBar;