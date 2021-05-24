import DestinationModel from '../models/destination-model.js';
import InstructionModel from '../models/instruction-model.js';

export const getDestination = async(req, res)=> {
    try {
        const getdest = await DestinationModel.find();

        res.status(200).json(getdest);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}


export const createDestination = async (req,res)=> {
    const dest = req.body;

    const newDest = new InstructionModel(dest);

    try {
        await newDest.save();

        res.status(201).json(newDest);
    } catch (error) {
        res.status(409).json({message: error.message}); 
    }
}
