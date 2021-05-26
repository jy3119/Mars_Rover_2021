import { FETCH_ALL, CREATE, DELETE } from '../constants/actionTypes';

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

export const deleteInstruction = (id) => async (dispatch) => {
  try {
    await api.deleteInstruction(id);

    dispatch({ type: DELETE, payload: id });
  } catch (error) {
    console.log(error.message);
  }
};