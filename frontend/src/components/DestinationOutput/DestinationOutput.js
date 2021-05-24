import React from 'react';
import { useDispatch, useSelector} from 'react-redux';
import useStyles from './styles';

const DestinationOutput = () => {
    //const dispatch = useDispatch();
    const outputs = useSelector((state)=>state.outputs);
    const classes = useStyles();

    console.log(outputs);
  
    return (
      <h1>DestinationOutput</h1>
    );
  };
  
  export default Post;
  