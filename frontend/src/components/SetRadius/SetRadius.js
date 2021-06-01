import React, { useEffect } from 'react';
import { Container, Grow, Grid } from '@material-ui/core';
import { useDispatch } from 'react-redux';  

/******** COMPONENTS *********/
import RadiusInput from './RadiusInput/RadiusInput';
import RadiusOutputs from './RadiusOutputs/RadiusOutputs';

/********** ACTIONS **********/
import { getRadius} from '../../actions/radiuses-actions';

const SetRadius = () => {
    const dispatch = useDispatch();

    useEffect(() => {
        dispatch(getRadius());
    }, [dispatch]);

    return (
      <Container maxWidth="lg">
        <Grow in>
          <Container>
            <Grid container justify="space-between" alignItems="stretch" spacing={3}>
              <Grid item xs={12} sm={4}>
                <RadiusInput />
              </Grid>
              <Grid item xs={12} sm={7}>
                <RadiusOutputs />
              </Grid>
            </Grid>
          </Container>
        </Grow>
      </Container>
    );
};

export default SetRadius;