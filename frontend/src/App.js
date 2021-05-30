import React from 'react';
import { Container } from '@material-ui/core';
import { BrowserRouter, Switch, Route } from 'react-router-dom';

/******** COMPONENTS *********/
import NavBar from './components/NavBar/Navbar';
//import RoverMap from './components/RoverMap';
import Home from './components/Home/Home'
import DestinationInput from './components/SetCoordinates/DestinationInput/DestinationInput';
import DirectionButton from './components/DirectionButton/DirectionButton';
import SetCoordinates from './components/SetCoordinates/SetCoordinates';

/*********** APP *************/
const App = () => {
  return (
    <BrowserRouter>
      <Container maxWidth="lg">
        <NavBar />
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/coord_mode" exact component={SetCoordinates} />
          <Route path="/direct_mode" exact component={DirectionButton} />
        </Switch>
      </Container>
    </BrowserRouter>
  );
};

export default App;
