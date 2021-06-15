import React from 'react';
import { Grid, CircularProgress } from '@material-ui/core';
import { useSelector } from 'react-redux';

import DirectionList from './DirectionList/DirectionList';
import useStyles from './styles';

const DirectionLists = () => {
  const instructions = useSelector((state) => state.instructions);
  const classes = useStyles();

  return (
    !instructions.length ? <CircularProgress /> : (
      <Grid className={classes.container} container alignItems="stretch" spacing={3}>
        {instructions.map((instruction) => (
          <Grid key={instruction._id} item lg={3}>
            <DirectionList instruction={instruction} />
          </Grid>
        ))}
      </Grid>
    )
  );
};

export default DirectionLists;