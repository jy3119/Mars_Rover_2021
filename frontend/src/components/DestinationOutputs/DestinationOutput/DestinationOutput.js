import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import DeleteIcon from '@material-ui/icons/Delete';
import EditIcon from '@material-ui/icons/Edit';
import moment from 'moment';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';

const DestinationOutput = () => {
  const dispatch = useDispatch();
  const classes = useStyles();
  const destination = useSelector((state)=>state.destinations.getLastInsertedDocument.find({}).sort({_id:-1}).limit(1));
  
  return (
    <Card className={classes.card}>
      <div className={classes.overlay}>
        <Typography variant="body2">{moment(destination.SetAt).fromNow()}</Typography>
      </div>
      <div className={classes.overlay2}>
        <Button style={{ color: 'white' }} size="small" onClick={() => {}}><EditIcon fontSize="default" /></Button>
      </div>
      <CardContent>
        <Typography variant="body2" color="textSecondary" component="p" align="center">{destination.x_coordinate}</Typography>
        <Typography variant="body2" color="textSecondary" component="p" align="center">{destination.y_coordinate}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => {}}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
  export default DestinationOutput;
  