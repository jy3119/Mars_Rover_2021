import express from 'express';
import mongoose from 'mongoose';

import RadiusModel from '../models/radius-model.js'

const router = express.Router();

export const getRadius = async(req, res)=> {
    try {
        const getrad = await RadiusModel.find();

        res.status(200).json(getrad);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}


export const createRadius = async (req,res)=> {
    const radius = req.body;

    const newRad = new RadiusModel(radius);

    try {
        await newRad.save();

        res.status(201).json(newRad);
    } catch (error) {
        res.status(409).json({message: error.message}); 
    }
}

export const deleteRadius = async (req, res) => {
    const { id } = req.params;

    if (!mongoose.Types.ObjectId.isValid(id)) return res.status(404).send(`No radius with id: ${id}`);

    await RadiusModel.findByIdAndRemove(id);

    res.json({ message: "Radius deleted successfully." });
}


export default router; 