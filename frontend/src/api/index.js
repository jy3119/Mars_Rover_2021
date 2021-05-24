import axios from 'axios';

const url = 'http://localhost:3000/instruction';

export const fetchInstructions = () => axios.get(url);
export const createInstruction = (newPost) => axios.post(url, newPost);
export const updateInstruction = (id, updatedPost) => axios.patch(`${url}/${id}`, updatedPost);