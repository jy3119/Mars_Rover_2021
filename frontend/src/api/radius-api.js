import axios from 'axios';

const url = 'http://localhost:3000/radius';

//return all instructions in database 
export const fetchRadius = () => axios.get(url); 
//create instruction in database 
export const createRadius = (newRad) => axios.post(url, newRad);
//delete destination in database 
export const deleteRadius = (id) => axios.delete(`${url}/${id}`);

