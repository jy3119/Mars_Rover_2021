import axios from 'axios';

const url = 'http://localhost:3000/instruction';

export const fetchInstructions = () => axios.get(url);
export const createPost = (newPost) => axios.post(url, newPost);
