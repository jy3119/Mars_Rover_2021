import mongoose from 'mongoose';
const Schema = mongoose.Schema

/*Map should contain x-coordinates, y-coordinates of obstacles*/ 
const InstructionList = new Schema(
    {
        index: {
            type: Number, 
            default: 0
        },
        direction: String,
    },
)

const Instructions = mongoose.model('Instructions', InstructionList);

export default Instructions;