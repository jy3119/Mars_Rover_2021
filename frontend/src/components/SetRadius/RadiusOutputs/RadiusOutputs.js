import React from 'react';
import { Grid, CircularProgress } from '@material-ui/core';
import { useSelector } from 'react-redux';

import RadiusOutput from './RadiusOutput/RadiusOutput';
import useStyles from './styles';

const RadiusOutputs = () => {
  const radiuses = useSelector((state) => state.radiuses);
  const classes = useStyles();

  return (
    !radiuses.length ? <CircularProgress /> : (
      <Grid className={radiuses.container} container alignItems="stretch" spacing={3}>
        {radiuses.map((radius) => (
          <Grid key={radius._id} item>
            <RadiusOutput radius={radius} />
          </Grid>
        ))}
      </Grid>
    )
  );
};

export default RadiusOutputs;