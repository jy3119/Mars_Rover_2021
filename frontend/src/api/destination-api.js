import axios from 'axios';

const url = 'http://localhost:3000/destination';

//return all destinations in database 
export const fetchDestination = () => axios.get(url); 
//create destination in database 
export const createDestination = (newDest) => axios.post(url, newDest);
//update destination in database
export const updateDestination = (id, updatedDestination) => axios.patch(`${url}/${id}`, updatedDestination);
//delete destination in database 
export const deleteDestination = (id) => axios.delete(`${url}/${id}`);