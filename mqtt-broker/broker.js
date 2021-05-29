//https://www.compose.com/articles/building-mongodb-into-your-internet-of-things-a-tutorial/ 

/********* Set up MQTT Broker *********/
var mqtt = require('mqtt'); //includes mqtt server 
//MONGODB
var mongodb = require('mongodb'); // includes mongoDB 
var mongodbClient = mongodb.MongoClient; //initialises the mongoDB client
var mongodbURI='mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var deviceRoot = "demo/device/"; //deviceroot is topic name given in arduino code 
var collection,client; //initialise collection and client

//AWS
var client = mqtt.connect("ws://18.188.43.23:9001", {
  //open connection with your broker in AWS via websocket
  username:"mqtt-broker", //authenticate your broker with username and password
  password:"coolbeans$4",
}); 

/********* Initialise Broker **********/
mongodbClient.connect(mongodbURI, setupCollection); //connect the database with collecion
     
/********* Broker Authentication **********/ 




/********* Broker Events **********/
//function that displays the data in the MongoDataBase

function setupCollection(err, db) {
    if(err) throw err;
    collection=db.collection("test_mqtt"); //name of the collection in the database
    client=mqtt.connect({ host: 'localhost', port: 1883 }); //connecting the mqtt server with the MongoDB database
    client.subscribe(deviceRoot+"+"); //subscribing to the topic name 
    client.on('message', insertEvent); //inserting the event
 }
 
 //function that displays the data in the MongoDataBase
function insertEvent(topic,message) {
	var key=topic.replace(deviceRoot,'');

	collection.update(
		{ _id:key }, 
		{ $push: { events: { event: {  value:message, when:new Date() } } } }, 
	 	{ upsert:true },
	 	function(err,docs) {
			if(err) {
				console.log("Insert fail");	// Improve error handling		
			}
		}
	);
}

//ESP32 part
sensor.read(11, 4, function (err, temperature, humidity) {
    //read sensor data from raspberry pi pin GPIO 4
    function timer(temp, humid) {
        client.publish(
          "mqtt/dht",
          JSON.stringify(`Temperature:${temp}C & Humidity:${humid}%`) //convert number to string
        ); //publish sensor data to broker on topic mqtt/dht
  
        console.log("topic published to the broker");
     
    }
  
    setInterval(timer, 5000, temperature, humidity); // send sensor data to broker every 5 seconds
  });



//AWS Public IPv4 Address: 18.188.43.23