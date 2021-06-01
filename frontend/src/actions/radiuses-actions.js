import { FETCH_ALL, CREATE, DELETE } from '../constants/actionTypes';

import * as api from '../api/radius-api.js';

/******* Action Creators ********/

export const getRadius = () => async (dispatch) => {
  try {
    const { data } = await api.fetchRadius();

    dispatch({ type: FETCH_ALL, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};


export const createRadius = (radius) => async (dispatch) => {
  try {
    const { data } = await api.createRadius(radius);

    dispatch({ type: CREATE, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};

export const deleteRadius = (id) => async (dispatch) => {
  try {
    await api.deleteRadius(id);

    dispatch({ type: DELETE, payload: id });
  } catch (error) {
    console.log(error.message);
  }
};