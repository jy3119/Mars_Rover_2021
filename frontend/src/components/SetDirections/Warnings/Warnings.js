import React from 'react';
import { Grid, CircularProgress } from '@material-ui/core';
import { useSelector } from 'react-redux';

import Warning from './Warning/Warning';
import useStyles from './styles';

const Warnings = () => {
  const classes = useStyles();

  return (
    <div>
      <Warning />
    </div>
  );
};

export default Warnings;