import React, { useEffect } from 'react';
import { Container, Grid, Grow } from '@material-ui/core';
import { useDispatch } from 'react-redux';

import DestinationInput from './DestinationInput/DestinationInput';
import DestinationOutputs from './DestinationOutputs/DestinationOutputs';

import { getDestination } from '../../actions/destinations-actions';

export const SetCoordinates = () => {
    const dispatch = useDispatch();

    useEffect(() => {
        dispatch(getDestination());
    }, [dispatch]);

    return (
      <Container maxWidth="lg">
        <Grow in>
          <Container>
            <Grid container justify="space-between" alignItems="stretch" spacing={3}>
              <Grid item xs={12} sm={4}>
                <DestinationInput />
              </Grid>
              <Grid item xs={12} sm={7}>
                <DestinationOutputs />
              </Grid>
            </Grid>
          </Container>
        </Grow>
      </Container>
    );
};

export default SetCoordinates;


