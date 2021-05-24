import React from 'react'
import AppBar from '@material-ui/core/AppBar'
import Toolbar from '@material-ui/core/Toolbar'
import Typography from '@material-ui/core/Typography'

const Header = () => {
    return(
        <div>
        <AppBar position="static">
            <Typography variant="h2" align ="center" color="inherit">
            MARS ROVER
            </Typography>
        </AppBar>
        </div>
    )
}
export default Header;