import mongoose from 'mongoose';
 
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
        },
        speed: {
            type: Number, 
            default: 5, //set to max first 
            max: 5 //can set proper value later 
        }
    }
);

var InstructionModel = mongoose.model('InstructionModel', InstructionSchema);

export default InstructionModel;