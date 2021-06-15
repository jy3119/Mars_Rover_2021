import express from 'express';

import { getInstruction, createInstruction, deleteInstruction } from '../controllers/instruction-ctrl.js' 

const router = express.Router()

//reached at http://localhost:3000/instruction

/******* Get Requests ********/
router.get('/', getInstruction);

/******* Post Requests ********/
router.post('/', createInstruction);

/******* Delete Requests ********/
router.delete('/:id', deleteInstruction);

export default router;