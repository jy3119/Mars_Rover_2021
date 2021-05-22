import Instructions from '../models/insruction-model.js'

export const getInstructions = async (req,res)=> {
    try {
        const getinstructions = await Instructions.find();

        res.status(200).json(getinstructions);
    } catch (error) {
        res.status(404).json({message: error.message});
    }
}

export const createInstructions = (req,res)=> {
    res.send('created Instruction');
}
