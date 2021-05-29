import React, { useEffect, useState, useSelector } from 'react';
import { Container } from '@material-ui/core';
import useStyles from './styles';
import { BrowserRouter, Switch, Route } from 'react-router-dom';

/******** COMPONENTS *********/
import NavBar from './components/NavBar/Navbar';
//import RoverMap from './components/RoverMap';
import Home from './components/Home/Home'

/*********** APP *************/
const App = () => {
  return (
    <BrowserRouter>
      <Container maxWidth="lg">
        <NavBar />
        <Switch>
          <Route path="/" exact component={Home} />
          <Route path="/auth" exact component={Auth} />
        </Switch>
        <Home />
      </Container>
    </BrowserRouter>
  );
};

export default App;
