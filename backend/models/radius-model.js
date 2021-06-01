import mongoose from 'mongoose';
 
const RadiusSchema = mongoose.Schema(
    {
        radius_dist: {
            type: Number, 
            default: 0, //set to max first 
            max: 2500 //can set proper value later 
        },
        SetAt: {
            type: Date, 
            default: new Date()
        }
    }
);

var RadiusModel = mongoose.model('RadiusModel', RadiusSchema);

export default RadiusModel;