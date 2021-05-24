import { FETCH_ALL, CREATE, UPDATE, DELETE, LIKE } from '../constants/actionTypes';

export default (instructions = [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    //case CREATE:
      //return [...instructions, action.payload];
    default:
      return instructions;
  }
};