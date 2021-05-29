import React from 'react';
import { Container, AppBar, Typography, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux';  

/******** COMPONENTS *********/
import DirectionButton from '../DirectionButton/DirectionButton.js';
import DestinationInput from '../DestinationInput/DestinationInput';
import DestinationOutputs from '../DestinationOutputs/DestinationOutputs';
import DirectionLists from '../DirectionLists/DirectionLists';

/********** ACTIONS **********/
import { getInstruction } from '../../actions/instructions-actions';
import { getDestination } from '../../actions/destinations-actions';

const Home = () => {

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
        <Grow in>
        <Container>
          <Grid container justify="space-between" alignItems="stretch" spacing={3}>
            <Grid>
              <DirectionButton />
            </Grid>
            <Grid>
              <DirectionLists />
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
      
    );
};

export default Home;