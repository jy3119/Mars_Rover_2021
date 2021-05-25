import express from 'express';

import { getDestination, createDestination, updateDestination} from '../controllers/destination-ctrl.js' 

const router = express.Router()

//reached at http://localhost:3000/destination
router.get('/', getDestination);

/******* Post Requests ********/
router.post('/', createDestination);

/******* Update Requests ********/
router.post('/:id', updateDestination);

export default router;