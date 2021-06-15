import axios from 'axios';

/*
const url = 'http://localhost:3000/instruction';

//return all instructions in database 
export const fetchInstruction = () => axios.get(url); 
//create instruction in database 
export const createInstruction = (newInstruct) => axios.post(url, newInstruct);
//delete destination in database 
export const deleteInstruction = (id) => axios.delete(`${url}/${id}`);
*/ 

const API = axios.create({ baseURL: 'https://tsersiew-design-project-2-2021.herokuapp.com/' });

export const fetchInstruction = () => API.get('/instruction');
export const createInstruction = (newInstruct) => API.post('/instruction', newInstruct);
export const deleteInstruction = (id) => API.delete(`/instruction/${id}`);


