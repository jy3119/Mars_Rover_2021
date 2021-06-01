import { FETCH_ALL, CREATE, DELETE } from '../constants/actionTypes';

export default (radiuses= [], action) => {
  switch (action.type) {
    case FETCH_ALL:
      return action.payload;
    case CREATE:
      return [...radiuses, action.payload];
    case DELETE:
      return radiuses.filter((radius) => radius._id !== action.payload);
    default:
      return radiuses;
  }
};