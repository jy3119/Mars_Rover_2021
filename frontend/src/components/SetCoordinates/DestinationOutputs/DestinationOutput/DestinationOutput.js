import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import DeleteIcon from '@material-ui/icons/Delete';
import EditIcon from '@material-ui/icons/Edit';
import moment from 'moment';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { deleteDestination } from '../../../../actions/destinations-actions';

const DestinationOutput = ({destination}) => {
  const dispatch = useDispatch();
  const classes = useStyles();
   
  return (
    <Card className={classes.card}>
      <div>
        <Typography variant="body2" color="textSecondary">{moment(destination.SetAt).format()}</Typography>
      </div>
      <CardContent>
        <Typography variant="body2" color="inherit" component="p" align="center">x_coordinate: {destination.x_coordinate}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">y_coordinate: {destination.y_coordinate}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => dispatch(deleteDestination(destination._id))}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
export default DestinationOutput;
  