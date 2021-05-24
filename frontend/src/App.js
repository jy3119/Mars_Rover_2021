import './App.css';
import React, { Component, useEffect } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux'; 

/******** COMPONENTS *********/
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';
import DestinationInput from './components/DestinationInput/DestinationInput.js';
import DestinationOutput from './components/DestinationOutput/DestinationOutput.js';

/********** ACTIONS **********/
//import { getInstruction } from './actions/instructions-actions';
import { getDestination } from './actions/destinations-actions';

/*********** APP *************/
const App = () => {
  const dispatch = useDispatch();

  useEffect(() => {
    dispatch(getDestination());
  }, [dispatch]);

  return (
    <div>
      <Header />
      <DirectionButton />
      <DestinationInput />
      <DestinationOutput />
    </div>
  );
};

export default App;
