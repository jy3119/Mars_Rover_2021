import React from 'react';
import { Container } from '@material-ui/core';
import { BrowserRouter, Switch, Route } from 'react-router-dom';
import { Connector } from 'mqtt-react-hooks'; //MQTT
import Status from './Status';

/******** COMPONENTS *********/
import NavBar from './components/NavBar/Navbar';
//import RoverMap from './components/RoverMap';
import Home from './components/Home/Home';
import SetDirections from './components/SetDirections/SetDirections';
import SetCoordinates from './components/SetCoordinates/SetCoordinates';
import SetRadius from './components/SetRadius/SetRadius';

/*********** APP *************/
const App = () => {
  return (
    <Connector brokerUrl="wss://test.mosquitto.org:8081">
    <BrowserRouter>
      <Container maxWidth="lg">
        <NavBar />
        <Status />
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/coord_mode" exact component={SetCoordinates} />
          <Route path="/direct_mode" exact component={SetDirections} />
          <Route path="/rad_mode" exact component={SetRadius} />
        </Switch>
      </Container>
    </BrowserRouter>
    </Connector>
  );
};

export default App;
