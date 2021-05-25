import express from 'express';

import { getInstruction, createInstruction } from '../controllers/instruction-ctrl.js' 

const router = express.Router()

//reached at http://localhost:3000/instruction

/******* Get Requests ********/
router.get('/', getInstruction);

/******* Post Requests ********/
router.post('/', createInstruction);

export default router;