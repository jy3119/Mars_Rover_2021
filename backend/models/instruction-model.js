import mongoose from 'mongoose';

/*Instruction should contain x-coordinates, y-coordinates of obstacles*/ 
const InstructionSchema = mongoose.Schema(
    {
        index: {type: Number, default: 0},
        direction: String,
    }
);

const InstructionModel = mongoose.model('InstructionModel', InstructionSchema);

export default InstructionModel;