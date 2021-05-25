import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import DeleteIcon from '@material-ui/icons/Delete';
import moment from 'moment';
import { useDispatch } from 'react-redux';

import useStyles from './styles';

const DestinationOutput = (destination) => {
  //const destinations = useSelector((state)=>state.destinations);
  const dispatch = useDispatch();
  const classes = useStyles();
  
  return (
    <Card className={classes.card}>
      <div className={classes.overlay}>
        <Typography variant="body2">{moment(destination.SetAt).fromNow()}</Typography>
      </div>
      <CardContent>
        <Typography variant="body2" color="textSecondary" component="p" align="center">{destination.x_coordinate}</Typography>
        <Typography variant="body2" color="textSecondary" component="p" align="center">{destination.y_coordinate}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => dispatch(deleteDestination(destination._id))}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
  export default DestinationOutput;
  