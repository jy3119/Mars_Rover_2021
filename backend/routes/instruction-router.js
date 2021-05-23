import express from 'express';

import { getInstruction, createInstruction } from '../controllers/instruction-ctrl.js' 

const router = express.Router()

//reached at localhost:3000/instruction
router.get('/', getInstruction);

 /**************POST REQUESTS **************/
 /*
 *	Brief: Post instructions for rover 
 *	Path: /api/plant/control/:plantId
 */

router.post('/', createInstruction);

export default router;