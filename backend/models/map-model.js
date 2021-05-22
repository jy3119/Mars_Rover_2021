const mongoose = require('mongoose')
const Schema = mongoose.Schema

/*Map should contain x-coordinates, y-coordinates of obstacles*/ 
const Map = new Schema(
    {
        name: { type: String, required: true },
        time: { type: [String], required: true },
        rating: { type: Number, required: true },
    },
    { timestamps: true },
)

module.exports = mongoose.model('rover', Map)