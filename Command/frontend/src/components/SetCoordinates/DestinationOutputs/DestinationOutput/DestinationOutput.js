import React from 'react';
import { Card, CardActions, CardContent, Button, Typography } from '@material-ui/core';
import DeleteIcon from '@material-ui/icons/Delete';
import moment from 'moment';
import { useDispatch } from 'react-redux';

import useStyles from './styles';
import { deleteDestination } from '../../../../actions/destinations-actions';

const DestinationOutput = ({destination}) => {
  const dispatch = useDispatch();
  const classes = useStyles();
   
  return (
    <Card className={classes.card} raised elevation={6}>
      <div>
        <Typography variant="body2" color="textSecondary">{moment(destination.SetAt).format()}</Typography>
      </div>
      <CardContent>
        <Typography variant="body2" color="inherit" component="p" align="center">x-coordinate: {destination.x_coordinate}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">y-coordinate: {destination.y_coordinate}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">radius: {destination.radius_dist}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => dispatch(deleteDestination(destination._id))}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
export default DestinationOutput;
  