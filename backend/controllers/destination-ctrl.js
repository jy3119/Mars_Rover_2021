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
/*
export const updateDestination = async (req, res) => {
    const { id } = req.params;
    const { x_coordinate, y_coordinate } = req.body;
    
    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No destination with id: ${id}`);

    const updatedDestination = { x_coordinate, y_coordinate, _id: id };

    await DestinationModel.findByIdAndUpdate(id, updatedDestination, { new: true });

    res.json(updatedDestination);
}*/


export const deleteDestination = async (req, res) => {
    const { id } = req.params;

    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No destination with id: ${id}`);

    await DestinationModel.findByIdAndRemove(id);

    res.json({ message: "Destination deleted successfully." });
}


export default router;