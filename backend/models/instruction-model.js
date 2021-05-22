const mongoose = require('mongoose')
const Schema = mongoose.Schema

/*Map should contain x-coordinates, y-coordinates of obstacles*/ 
const Map = new Schema(
    {
        type_colour: { type: String, required: true }, //ping pong balls of different colours 
        time: { type: [String], required: true },
        x_coordinate: { type: Number, required: true },
        y_coordinate: { type: Number, required: true },
    },
    { timestamps: true },
)

module.exports = mongoose.model('rover', Map)