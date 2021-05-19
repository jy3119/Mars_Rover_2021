const mongoose = require('mongoose')

/* Connecting to MongoDB */
mongoose
    .connect('mongodb://127.0.0.1:27017/cinema', { useNewUrlParser: true, useCreateIndex: true, useUnifiedTopology: true, useFindAndModify: false})
    console.log('MongoDB connected')
    .catch(e => {
        console.error('Connection error', e.message)
    })

const db = mongoose.connection

module.exports = db


/*
$ mkdir -p /data/db
$ brew services start mongodb
$ mongo
> use cinema

do outside of backend folder
*/


/*
import mongoose from 'mongoose'
import config from 'config'

const db: string = config.get('mongoUri')

async function connectDB() {
	try {
		await mongoose.connect(db, {
			useNewUrlParser: true,
			useCreateIndex: true,
			useUnifiedTopology: true,
			useFindAndModify: false,
		})
		console.log('MongoDB connected')
	} catch (err) {
		console.log(err.message)
		process.exit(1)
	}
}

export default connectDB
*/
