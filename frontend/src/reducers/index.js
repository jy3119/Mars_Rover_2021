import { combineReducers } from 'redux';

import instructions from './instructions-reducer';
import destinations from './destinations-reducer';

export const reducers = combineReducers({ 
    instructions, 
    destinations,
});

