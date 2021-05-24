import React from 'react';
import { useDispatch, useSelector} from 'react-redux';
import useStyles from './styles';

const DestinationOutput = () => {
    //const dispatch = useDispatch();
    const destinations = useSelector((state)=>state.destinations);
    const classes = useStyles();

    console.log(destinations);
  
    return (
      <h1>DestinationOutput</h1>
    );
  };
  
  export default DestinationOutput;
  