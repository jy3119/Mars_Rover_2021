import axios from 'axios';

/*
const url = 'http://localhost:3000/destination';

//return all destinations in database 
export const fetchDestination = () => axios.get(url); 
//create destination in database 
export const createDestination = (newDest) => axios.post(url, newDest);
//update destination in database
//export const updateDestination = (id, updatedDestination) => axios.patch(`${url}/${id}`, updatedDestination);
//delete destination in database 
export const deleteDestination = (id) => axios.delete(`${url}/${id}`);
*/

const API = axios.create({ baseURL: 'https://tsersiew-design-project-2-2021.herokuapp.com/' });

export const fetchDestination = () => API.get('/destination'); 
export const createDestination = (newDest) => API.post('/destination',newDest);
export const deleteDestination = (id) => API.delete(`/destination/${id}`);