import express from 'express';
import mongoose from 'mongoose';

import InstructionModel from '../models/instruction-model.js'

const router = express.Router();

export const getInstruction = async(req, res)=> {
    try {
        const getinstruct = await InstructionModel.find();

        res.status(200).json(getinstruct);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}


export const createInstruction = async (req,res)=> {
    const instruction = req.body;

    const newInstruct = new InstructionModel(instruction);

    try {
        await newInstruct.save();

        res.status(201).json(newInstruct);
    } catch (error) {
        res.status(409).json({message: error.message}); 
    }
}

export default router; 