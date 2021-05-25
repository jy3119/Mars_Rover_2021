import mongoose from 'mongoose';

/*Instruction should contain x-coordinates, y-coordinates of obstacles*/ 
const InstructionSchema = mongoose.Schema(
    {
        direction: String,
        distance: {
            type: Number, 
            default: 0
        },
        angle: {
            type: Number, 
            default: 0
        }
    }
);

var InstructionModel = mongoose.model('InstructionModel', InstructionSchema);

export default InstructionModel;