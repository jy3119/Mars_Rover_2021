import express from 'express';
import mongoose from 'mongoose';

import DestinationModel from '../models/destination-model.js';

const router = express.Router();

export const getDestination = async(req, res)=> {
    try {
        const getdest = await DestinationModel.find();

        res.status(200).json(getdest);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}

export const createDestination = async (req, res) => {
    const destination = req.body;

    const newDest = new DestinationModel(destination);

    try {
        await newDest.save();

        res.status(201).json(newDest);
    } catch (error) {
        res.status(409).json({ message: error.message });
    }
}

export default router;