import axios from 'axios';

const url = 'http://localhost:3000/instruction';

//return all instructions in database 
export const fetchInstruction = () => axios.get(url); 
//create instruction in database 
export const createInstruction = (newInstruct) => axios.post(url, newInstruct);



