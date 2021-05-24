import express from 'express';

import { getDestination, createDestination } from '../controllers/destination-ctrl.js' 

const router = express.Router()

//reached at http://localhost:3000/destination
router.get('/', getDestination);

 /**************POST REQUESTS **************/
 /*
 *	Brief: Post Destination for rover 
 */

router.post('/', createDestination);

export default router;