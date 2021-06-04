import mongoose from 'mongoose';

/*Destination should contain x-coordinates, y-coordinates*/ 
const DestinationSchema = mongoose.Schema(
    {
        x_coordinate: Number, 
        y_coordinate: Number, 
        radius_dist: {
            type: Number, 
            default: 0, //set to max first 
            max: 2500 //can set proper value later 
        },
        SetAt: {
            type: Date, 
            default: new Date()
        },
    }
);

var DestinationModel = mongoose.model('DestinationModel', DestinationSchema);

export default DestinationModel;