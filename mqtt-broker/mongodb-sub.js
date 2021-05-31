//MQTT Subscriber that writes to MongoDB 

/********* MQTT Broker ***********/
import mqtt from 'mqtt';
/********* mongdoDB Client ********/
import mongodb from 'mongodb'; // includes mongoDB 
import mongoose from 'mongoose';
//var mongodbClient = mongodb.MongoClient; //initialises the mongoDB client
//mongodbURI='mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
//var deviceRoot = "test"; //deviceroot is topic name given in arduino code eg. /esp32
//var collection; //initialise collection and client

const CONNECTION_URL = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';

mongoose.connect(CONNECTION_URL, {useNewUrlParser: true, useUnifiedTopology: true})
    .then(()=>{setupCollection});

mongoose.set('useFindAndModify', false);

/*********Connect to MongoDB Database ***********/
//mongodbClient.connect(mongodbURI, setupCollection); //connect the database with collection
     
function setupCollection(err, db) {
   if(err) throw err;
   //collection=db.collection("obstacles"); //name of the collection in the database
   collection=db.collection("test"); //name of the collection in the database
   var client = mqtt.connect("ws://18.116.230.109:9001", {
      //open connection with your broker in AWS via websocket
      username:"mqtt-broker", //authenticate your broker with username and password
      password:"coolbeans$4",
      });  //connecting the mqtt server with the MongoDB database
   client.subscribe(deviceRoot+"+"); //subscribing to the topic name demo/device/+
   //client.on('message', insertEvent); //inserting the event
   client.on('message',insertEvent);
}

//function that displays the data in the MongoDataBase
/*
function insertEvent(topic,payload) {
  //var key=topic.replace(deviceRoot,''); //uses device name as key i.e. esp32
  parse_string = payload.toString(); 
  parse_coord = parse_string.split(',');
  x_coord = parse_coord[0];
  y_coord = parse_coord[1];

  collection.insert(
   { x_coordinate: x_coord, y_coordinate: y_coord, when:new Date() }, 
   //{ upsert:true },

   function(err,docs) {  
      if(err) {
         console.log("Insert fail")// Improve error handling		
      }
   }
   );
}*/

function insertEvent(topic,payload) {
   collection.insert(
      { value:payload, when:new Date() }, 
      //{ upsert:true },
   
      function(err,docs) {  
         if(err) {
            console.log("Insert fail")// Improve error handling		
         }
      }
      );
}