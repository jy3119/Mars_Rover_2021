import { FETCH_ALL } from '../constants/actionTypes';

import * as api from '../api/index.js';

/******* Action Creators ********/

export const getInstruction = () => async (dispatch) => {
  try {
    const { data } = await api.fetchInstructions();

    dispatch({ type: FETCH_ALL, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};

/*
export const createInstruction = (post) => async (dispatch) => {
  try {
    const { data } = await api.createInstruction(post);

    dispatch({ type: CREATE, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};*/

