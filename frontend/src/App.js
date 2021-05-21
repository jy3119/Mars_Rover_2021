import './App.css';
import React, { Component } from 'react';
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton';


class App extends Component {
  render() {
    return (
      <div>
        <Header />
        <DirectionButton />
      </div>
    )
  }
}

export default App;
