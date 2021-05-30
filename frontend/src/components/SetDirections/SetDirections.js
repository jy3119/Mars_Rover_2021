import React, { useEffect } from 'react';
import { Container, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux';  

/******** COMPONENTS *********/
import DirectionButton from './DirectionButton/DirectionButton';
import DirectionLists from './DirectionLists/DirectionLists';

/********** ACTIONS **********/
import { getInstruction } from '../../actions/instructions-actions';

const SetDirections = () => {
    const dispatch = useDispatch();

    useEffect(() => {
        dispatch(getInstruction());
    }, [dispatch]);

    return (
      <Container maxWidth="lg">
        <Grow in>
          <Container>
            <Grid container justify="space-between" alignItems="stretch" spacing={3}>
              <Grid item xs={12} sm={4}>
                <DirectionButton />
              </Grid>
              <Grid item xs={12} sm={7}>
                <DirectionLists />
              </Grid>
            </Grid>
          </Container>
        </Grow>
      </Container>
    );
};

export default SetDirections;