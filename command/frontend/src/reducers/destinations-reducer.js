import { FETCH_ALL, CREATE, UPDATE, DELETE } from '../constants/actionTypes';

export default (destinations= [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    case CREATE:
      return [...destinations, action.payload];
    case UPDATE:
      return destinations.map((destination) => (destination._id === action.payload._id ? action.payload : destination));
    case DELETE:
      return destinations.filter((destination) => destination._id !== action.payload);
    default:
      return destinations;
  }
};