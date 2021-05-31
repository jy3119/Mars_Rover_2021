import React from 'react';
import { Container } from '@material-ui/core';
import { BrowserRouter, Switch, Route } from 'react-router-dom';
import { Connector } from 'mqtt-react-hooks'; //MQTT

/******** COMPONENTS *********/
import NavBar from './components/NavBar/Navbar';
//import RoverMap from './components/RoverMap';
import Home from './components/Home/Home';
import SetDirections from './components/SetDirections/SetDirections';
import SetCoordinates from './components/SetCoordinates/SetCoordinates';

/*********** APP *************/
const App = () => {
  return (
    <Connector brokerUrl="mqtt://ec2-18-191-210-250.us-east-2.compute.amazonaws.com:1883">
    <BrowserRouter>
      <Container maxWidth="lg">
        <NavBar />
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/coord_mode" exact component={SetCoordinates} />
          <Route path="/direct_mode" exact component={SetDirections} />
        </Switch>
      </Container>
    </BrowserRouter>
    </Connector>
  );
};

export default App;
