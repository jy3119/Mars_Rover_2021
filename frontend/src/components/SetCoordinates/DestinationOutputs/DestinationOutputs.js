import React from 'react';
import { Grid, CircularProgress } from '@material-ui/core';
import { useSelector } from 'react-redux';

import DestinationOutput from './DestinationOutput/DestinationOutput';
import useStyles from './styles';

const DestinationOutputs = ({ setCurrentId }) => {
  const destinations = useSelector((state) => state.destinations);
  const classes = useStyles();

  return (
    !destinations.length ? <CircularProgress /> : (
      <Grid className={classes.container} container alignItems="stretch" spacing={3}>
        {destinations.map((destination) => (
          <Grid key={destination._id} item>
            <DestinationOutput destination={destination} setCurrentId={setCurrentId} />
          </Grid>
        ))}
      </Grid>
    )
  );
};

export default DestinationOutputs;
