import mqtt from 'mqtt';
import mongodb from 'mongodb';

const uri = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var mongodbClient = mongodb.MongoClient(uri,{ useUnifiedTopology: true, useNewUrlParser: true }); //initialises the mongoDB client 

const dbName = "mapdb";

var q = [0,0];
var distance = 0; 

async function distance_map () {
    try {
        await mongodbClient.connect();
        console.log("Connected to mongodb atlas");
        const db = mongodbClient.db(dbName);
        /* Use the collection "test" */
        const col = db.collection("distancemodels");
        var mqtt_client = mqtt.connect("mqtt://ec2-18-223-15-156.us-east-2.compute.amazonaws.com", {port:1883});

        /* setup the callbacks */
        mqtt_client.on('connect', function () {
            console.log('Connected to mqtt broker');
        });
        mqtt_client.on('error', function (error) {
            console.log(error);
        });
        
        /* parsing the message and inserting into mongodb 
            Publishing from esp32 to topic
            1 topic: liveloc
            send in this format: x,y
        */ 
        
        mqtt_client.on('message', function (topic, message) {
            //Called each time a message is received
            // Insert a single document, wait for promise so we can read it back
                //coordx, coordy
                var parse_string = message.toString(); 
                var parse_coord = parse_string.split(',');
                var rover_x = Number(parse_coord[0]); 
                var rover_y = Number(parse_coord[1]); 
                
                q.push(rover_x);
                q.push(rover_y); 
                if (q.length>4) {
                    q.shift();
                    q.shift();
                }
                var vert = Math.pow(q[3]-q[1],2);
                var hor = Math.pow(q[2]-q[0],2);
                console.log(vert,hor);
                distance = Math.sqrt(vert+hor);
                const p = col.insertOne({ dist: distance, timestamp: new Date()});
    
            console.log('Received message:', topic, message.toString());
        });

         /* subscribe to topic 'my/test/topic' */
         mqtt_client.subscribe('liveloc');
        
         /* publish message 'Hello' to topic 'my/test/topic' */
         mqtt_client.publish('liveloc', '58,12');
         mqtt_client.publish('liveloc', '45,17');
         mqtt_client.publish('liveloc', '58,34');
         mqtt_client.publish('liveloc', '66,14');
         mqtt_client.publish('liveloc', '49,12');
         mqtt_client.publish('liveloc', '55,27');
        
       } catch (err) {
        console.log(err.stack);
    }
}

distance_map().catch(console.dir);