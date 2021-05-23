import './App.css';
import React, { Component } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux'; 

import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';
import DestinationInput from './components/DestinationInput';

import { getInstruction } from './actions/instructions';

const dispatch = useDispatch();

class App extends Component {
  render() {

    useEffect(() => {
      dispatch(getInstruction());
    }, [dispatch]);

    return (
      <div>
        <Header />
        <DirectionButton />
        <DestinationInput />
      </div>
    )
  }
}

export default App;
