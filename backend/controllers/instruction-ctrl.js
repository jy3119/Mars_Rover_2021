import InstructionModel from '../models/instruction-model.js'

export const getInstruction = async(req, res)=> {
    try {
        const getinstructions = await InstructionModel.find();

        res.status(200).json(getinstructions);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}


export const createInstruction = async (req,res)=> {
    const direction = req.body;

    const new_direction = new Instructions(post);

    try {
        await new_direction.save();

        res.status(201).json(new_direction);
    } catch (error) {
        res.status(409).json({message: error.message}); 
    }
}
