import React from 'react';
import { Card, CardActions, CardContent, CardMedia, Button, Typography } from '@material-ui/core/';
import DeleteIcon from '@material-ui/icons/Delete';
import moment from 'moment';
import { useDispatch, useSelector } from 'react-redux';

import useStyles from './styles';
import { deleteInstruction } from '../../../../actions/instructions-actions';

const DirectionList = ({instruction}) => {
  const dispatch = useDispatch();
  const classes = useStyles();
  //const destination = useSelector((state)=>state.destinations.getLastInsertedDocument.find({}).sort({_id:-1}).limit(1));
   
  return (
    <Card className={classes.card} raised elevation={6}>
      <div>
      <Typography variant="body2" color="textSecondary">{moment(instruction.SetAt).format()}</Typography>
      </div>
      <CardContent>
        <Typography variant="body2" color="inherit" component="p" align="center">direction: {instruction.direction}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">speed: {instruction.speed}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">angle: {instruction.angle}</Typography>
        <Typography variant="body2" color="inherit" component="p" align="center">distance: {instruction.distance}</Typography>
      </CardContent>
      <CardActions className={classes.cardActions}>
        <Button size="small" color="primary" onClick={() => dispatch(deleteInstruction(instruction._id))}><DeleteIcon fontSize="small" /> Delete</Button>
      </CardActions>
    </Card>
  );
};
  
  
export default DirectionList;