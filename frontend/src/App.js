import './App.css';
import React, { Component } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';
import DestinationInput from './components/DestinationInput';


class App extends Component {
  render() {
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
