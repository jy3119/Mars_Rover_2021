import mongoose from 'mongoose';

/*Destination should contain x-coordinates, y-coordinates*/ 
const DestinationSchema = mongoose.Schema(
    {
        x_coordinate: Number, 
        y_coordinate: Number, 
        SetAt: {
            type: Date, 
            default: new Date()
        },
    }
);

const DestinationModel = mongoose.model('DestinationModel', DestinationSchema);

export default DestinationModel;