import express from 'express';

import { getDestination, createDestination, deleteDestination} from '../controllers/destination-ctrl.js' 

const router = express.Router()

//reached at http://localhost:3000/destination

/******* Get Requests ********/
router.get('/', getDestination);

/******* Post Requests ********/
router.post('/', createDestination);

/******* Update Requests ********/
//router.patch('/:id', updateDestination);

/******* Delete Requests ********/
router.delete('/:id', deleteDestination);

export default router;