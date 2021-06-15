import { FETCH_ALL, CREATE, DELETE } from '../constants/actionTypes';

export default (destinations= [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    case CREATE:
      return [...destinations, action.payload];
    case DELETE:
      return destinations.filter((destination) => destination._id !== action.payload);
    default:
      return destinations;
  }
};