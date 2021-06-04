/********************** INITIAL BACKEND APPLICATION SET-UP *******************/
import express from 'express';
import bodyParser from 'body-parser';
import mongoose from 'mongoose';
import cors from 'cors';

/* MQTT imports */
import mqtt from 'mqtt';

/*Backend Routes*/ 
import InstructionRouter from './routes/instruction-router.js';
import DestinationRouter from './routes/destination-router.js';
//const MapRouter = require('./routes/map-router')

const server = express();

/* Middleware for parsing */
server.use(bodyParser.json({ extended: true }));
server.use(bodyParser.urlencoded({ extended: true }));
server.use(cors());

server.use('/instruction', InstructionRouter);
server.use('/destination', DestinationRouter);
//server.use(MapRouter)

/* Get port */
const PORT = process.env.PORT || 3000;


/************************ CONNECTION TO MONGO DATABASE **********************/
const CONNECTION_URL = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';

mongoose.connect(CONNECTION_URL, {useNewUrlParser: true, useUnifiedTopology: true})
    .then(()=>server.listen(PORT, ()=>console.log(`Server running on port: ${PORT}`)))
    .catch((error)=>console.log(error.message));

mongoose.set('useFindAndModify', false);


/************** CONNECTION TO MQTT BROKER AND MESSAGE HANDLING ***************/
/* description of broker 
    Amazon EC2 Linux Instance 
    equivalent of having local gateway running Linux Distribution 
*/

/* Connecting to mqtt broker */
/*
var client = mqtt.connect("ws://18.188.43.23:9001", {
      //open connection with your broker in AWS via websocket
      username:"mqtt-broker", //authenticate your broker with username and password
      password:"coolbeans$4",
    });  //connecting the mqtt server with the MongoDB database
*/

/* Subscribe and publish to topics and handle messages */
/*
client.on('connect', async () => {
	console.log('Subscriber connected!')

	try {
        
	} catch (error) {
		console.log(error)
	}
})*/

/********** Socket.io Connection 
const { Server } = require("socket.io"); 
const io = new Server(server);

const obstacles = []; //get from mongodb database 

io.on('connection', (socket) => {
    console.log('a user connected');
    for (let i=0; i<obstacles.length; i++) {
        socket.emit('obstacle', obstacles[i]);
    }
    socket.on('obstacle', data => {
        obstacles.push(data);
        io.emit('obstacle', data);
    })
});
*/