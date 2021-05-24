import { FETCH_ALL, CREATE, UPDATE } from '../constants/actionTypes';

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


export const createDestination = (post) => async (dispatch) => {
  try {
    const { data } = await api.createDestination(post);

    dispatch({ type: CREATE, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};

/*
export const updateInstruction = (id, post) => async (dispatch) => {
    try {
      const { data } = await api.updateInstruction(id, post);
  
      dispatch({ type: UPDATE, payload: data });
    } catch (error) {
      console.log(error.message);
    }
  };
*/