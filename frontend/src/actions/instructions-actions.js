import { FETCH_ALL, CREATE, UPDATE } from '../constants/actionTypes';

import * as api from '../api/instruction-api.js';

/******* Action Creators ********/

export const getInstruction = () => async (dispatch) => {
  try {
    const { data } = await api.fetchInstruction();

    dispatch({ type: FETCH_ALL, payload: data });
  } catch (error) {
    console.log(error.message);
  }
};


export const createInstruction = (instruction) => async (dispatch) => {
  try {
    const { data } = await api.createInstruction(instruction);

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