import express from 'express';

import { getInstructions, createInstruction } from '../controllers/instruction-ctrl.js' 

const router = express.Router()

//reached at localhost:3000/instruction
router.get('/', getInstructions);

 /**************POST REQUESTS **************/
 /*
 *	Brief: Post instructions for rover 
 *	Path: /api/plant/control/:plantId
 */

 router.post('/', createInstruction)

export default router;