//MQTT Subscriber that writes to MongoDB 

/********* MQTT Broker ***********/
var mqtt = require('mqtt'); //includes mqtt server 
/********* mongdoDB Client ********/
var mongodb = require('mongodb'); // includes mongoDB 
var mongodbClient = mongodb.MongoClient; //initialises the mongoDB client
mongodbURI='mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var deviceRoot = "demo/device/"; //deviceroot is topic name given in arduino code eg. /esp32
var collection; //initialise collection and client


/*********Connect to MongoDB Database ***********/
mongodbClient.connect(mongodbURI, setupCollection); //connect the database with collecion
     
function setupCollection(err, db) {
   if(err) throw err;
   collection=db.collection("obstacles"); //name of the collection in the database
   var client = mqtt.connect("ws://18.188.43.23:9001", {
      //open connection with your broker in AWS via websocket
      username:"mqtt-broker", //authenticate your broker with username and password
      password:"coolbeans$4",
      });  //connecting the mqtt server with the MongoDB database
   client.subscribe(deviceRoot+"+"); //subscribing to the topic name demo/device/+
   client.on('message', insertEvent); //inserting the event
}

//function that displays the data in the MongoDataBase
function insertEvent(topic,payload) {
  var key=topic.replace(deviceRoot,''); //uses device name as key i.e. esp32

  collection.update(
  { _id:key }, 
  { $push: { events: { event: {  value:payload, when:new Date() } } } }, 
  { upsert:true },

  function(err,docs) {  
  if(err) {
     console.log("Insert fail")// Improve error handling		
  }
}

);

}