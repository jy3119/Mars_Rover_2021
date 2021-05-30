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
        <Grow in>
        <Container>
          <Grid container justify="space-between" alignItems="stretch" spacing={3}>
            <Grid>
              <DirectionButton />
            </Grid>
            <Grid>
              <DirectionLists />
            </Grid>
          </Grid>
        </Container>
      </Grow>
    );
};

export default SetDirections;