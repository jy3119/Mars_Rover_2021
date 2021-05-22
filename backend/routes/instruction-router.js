import express from 'express';

const router = express.Router()

//reached at localhost:3000/instruction
router.get('/', (req,res)=> {
    res.send('THIS WORKS');
});

export default router;