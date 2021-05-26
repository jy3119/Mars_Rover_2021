import { FETCH_ALL, CREATE, UPDATE, DELETE } from '../constants/actionTypes';

export default (instructions= [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    case CREATE:
      return [...instructions, action.payload];
    case DELETE:
      return instructions.filter((instruction) => instruction._id !== action.payload);
    default:
      return instructions;
  }
};