import React, { useEffect, useState, useSelector } from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux'; 
import useStyles from './styles';

/******** COMPONENTS *********/
import Header from './components/Header';
//import RoverMap from './components/RoverMap';
import DirectionButton from './components/DirectionButton/DirectionButton.js';
import DestinationInput from './components/DestinationInput/DestinationInput';
import DestinationOutputs from './components/DestinationOutputs/DestinationOutputs';

/********** ACTIONS **********/
import { getInstruction } from './actions/instructions-actions';
import { getDestination } from './actions/destinations-actions';


/*********** APP *************/
const App = () => {
  const [currentId, setCurrentId] = useState(0);
  const classes = useStyles(); 
  const dispatch = useDispatch();

  useEffect(() => {
    dispatch(getDestination());
  }, [dispatch]);

  useEffect(() => {
    dispatch(getInstruction());
  }, [dispatch]);

  return (
    <div>
      <Header />
      <Grow in>
        <Container>
          <Grid container justify="space-between" alignItems="stretch" spacing={3}>
            <Grid>
              <DirectionButton />
            </Grid>
            <Grid item xs={12} sm={7}>
              <DestinationOutputs setCurrentId={setCurrentId}/>
            </Grid>
            <Grid item xs={12} sm={4}>
              <DestinationInput currentId={currentId} setCurrentId={setCurrentId}/>
            </Grid>
          </Grid>
        </Container>
      </Grow>
      
    </div>
  );
};

export default App;
