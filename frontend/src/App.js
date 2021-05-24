import './App.css';
import React, { Component, useEffect } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux'; 

/******** COMPONENTS *********/
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';
import DestinationInput from './components/DestinationInput/DestinationInput.js';

/********** ACTIONS **********/
import { getInstruction } from './actions/instructions-actions';

/*********** APP *************/
const App = () => {
  const dispatch = useDispatch();

  useEffect(() => {
    dispatch(getInstruction());
  }, [dispatch]);

  return (
    <div>
      <Header />
      <DirectionButton />
      <DestinationInput />
    </div>
  );
};

export default App;
