/********************** INITIAL BACKEND APPLICATION SET-UP *******************/
import express from 'express';
import bodyParser from 'body-parser';
import mongoose from 'mongoose';
import cors from 'cors';

/*Backend Routes*/ 
import InstructionRouter from './routes/instruction-router.js';
import DestinationRouter from './routes/destination-router.js';

const server = express();

/* Middleware for parsing */
server.use(bodyParser.json({ extended: true }));
server.use(bodyParser.urlencoded({ extended: true }));
server.use(cors());

server.use('/instruction', InstructionRouter);
server.use('/destination', DestinationRouter);

server.get('/', (req,res) => {
    res.send('APP IS RUNNING.');
});

/* Get port */
const PORT = process.env.PORT || 3000;

/************************ CONNECTION TO MONGO DATABASE **********************/
const CONNECTION_URL = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';

mongoose.connect(CONNECTION_URL, {useNewUrlParser: true, useUnifiedTopology: true})
    .then(()=>server.listen(PORT, ()=>console.log(`Server running on port: ${PORT}`)))
    .catch((error)=>console.log(error.message));

mongoose.set('useFindAndModify', false);



