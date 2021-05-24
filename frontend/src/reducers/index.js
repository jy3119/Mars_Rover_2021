import { combineReducers } from 'redux';

import instructions from './instructions-reducer';
import destinations from './destinations-reducer';

const reducers = combineReducers({ 
    instructions, 
    destinations,
});

export default reducers;
