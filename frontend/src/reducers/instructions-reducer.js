import { FETCH_ALL, CREATE, UPDATE, DELETE } from '../constants/actionTypes';

export default (instructions= [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    case CREATE:
      return [...instructions, action.payload];
    /*
    case UPDATE:
      return instructions.map((destination) => (destination._id === action.payload._id ? action.payload : destination));
    case DELETE:
      return instructions.filter((destination) => destination._id !== action.payload);*/
    default:
      return instructions;
  }
};