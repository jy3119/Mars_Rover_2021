import { combineReducers } from 'redux';

import instructions from './instructions-reducer';
import destinations from './destinations-reducer';
import radiuses from './radiuses-reducer';

const reducers = combineReducers({ 
    instructions, 
    destinations,
    radiuses, 
});

export default reducers;
