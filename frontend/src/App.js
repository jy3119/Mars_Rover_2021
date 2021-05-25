import React, { useEffect, useState, useSelector } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux'; 
import useStyles from './styles';

/******** COMPONENTS *********/
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';
import DestinationInput from './components/DestinationInput/DestinationInput';
import DestinationOutput from './components/DestinationOutputs/DestinationOutput/DestinationOutput';

/********** ACTIONS **********/
//import { getInstruction } from './actions/instructions-actions';
import { getDestination } from './actions/destinations-actions';


/*********** APP *************/
const App = () => {
  const [currentId, setCurrentId] = useState(0);
  const classes = useStyles(); 
  const dispatch = useDispatch();

  useEffect(() => {
    dispatch(getDestination());
  }, [dispatch]);

  return (
    <div>
      <Header />
      <DirectionButton />
      <DestinationInput currentId={currentId} setCurrentId={setCurrentId}/>
      <DestinationOutput setCurrentId={setCurrentId}/>
    </div>
  );
};

export default App;
