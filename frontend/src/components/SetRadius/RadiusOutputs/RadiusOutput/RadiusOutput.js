import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import DeleteIcon from '@material-ui/icons/Delete';
import moment from 'moment';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { deleteRadius } from '../../../../actions/radiuses-actions';

const RadiusOutput = ({radius}) => {
  const dispatch = useDispatch();
  const classes = useStyles();
  //const destination = useSelector((state)=>state.destinations.getLastInsertedDocument.find({}).sort({_id:-1}).limit(1));
   
  return (
    <Card className={classes.card}>
      <div>
      <Typography variant="body2" color="textSecondary">{moment(radius.SetAt).format()}</Typography>
      </div>
      <CardContent>
        <Typography variant="body2" color="inherit" component="p" align="center">radius: {radius.radius_dist}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => dispatch(deleteRadius(radius._id))}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
export default RadiusOutput;