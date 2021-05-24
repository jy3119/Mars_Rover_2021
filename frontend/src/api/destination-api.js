import axios from 'axios';

const url = 'http://localhost:3000/destination';

//return all destinations in database 
export const fetchDestination = () => axios.get(url); 
//create destination in database 
export const createDestination = (newDest) => axios.post(url, newDest);
//export const updateInstruction = (id, updatedPost) => axios.patch(`${url}/${id}`, updatedPost);
