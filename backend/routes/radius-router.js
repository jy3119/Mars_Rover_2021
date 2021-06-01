import express from 'express';

import { getRadius, createRadius, deleteRadius } from '../controllers/radius-ctrl.js';

const router = express.Router()

//reached at http://localhost:3000/instruction

/******* Get Requests ********/
router.get('/', getRadius);

/******* Post Requests ********/
router.post('/', createRadius);

/******* Delete Requests ********/
router.delete('/:id', deleteRadius);

export default router;