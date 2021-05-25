import { FETCH_ALL, CREATE, UPDATE, DELETE} from '../constants/actionTypes';

import * as api from '../api/destination-api.js';

/******* Action Creators ********/
export const getDestination = () => async (dispatch) => {
  try {
    const { data } = await api.fetchDestination();

    dispatch({ type: FETCH_ALL, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};


export const createDestination = (destination) => async (dispatch) => {
  try {
    const { data } = await api.createDestination(destination);

    dispatch({ type: CREATE, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};


export const updateDestination = (id, destination) => async (dispatch) => {
    try {
      const { data } = await api.updateDestination(id, destination);
  
      dispatch({ type: UPDATE, payload: data });
    } catch (error) {
      console.log(error.message);
    }
  };


export const deleteDestination = (id) => async (dispatch) => {
  try {
    await api.deleteDestination(id);

    dispatch({ type: DELETE, payload: id });
  } catch (error) {
    console.log(error.message);
  }
};