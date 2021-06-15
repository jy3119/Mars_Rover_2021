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
            default: 5, 
            max: 5 
        },
        SetAt: {
            type: Date, 
            default: new Date()
        }
    }
);

var InstructionModel = mongoose.model('InstructionModel', InstructionSchema);

export default InstructionModel;