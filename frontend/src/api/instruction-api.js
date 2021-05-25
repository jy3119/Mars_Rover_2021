import axios from 'axios';

const url = 'http://localhost:3000/instruction';

//return all destinations in database 
export const fetchInstruction = () => axios.get(url); 
//create destination in database 
export const createInstruction = (newInstruct) => axios.post(url, newInstruct);



