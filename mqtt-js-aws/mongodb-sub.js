var mqtt = require('mqtt');
var mongodb = require('mongodb');

const uri = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var mongodbClient = mongodb.MongoClient(uri,{ useUnifiedTopology: true, useNewUrlParser: true }); //initialises the mongoDB client 

const dbName = "mapdb";

async function main () {
    try {
        await mongodbClient.connect();
        console.log("Connected to mongodb atlas");
        const db = mongodbClient.db(dbName);
        /* Use the collection "test" */
        const col = db.collection("mapmodels");
        /*var options = {
            host: '076cff12ed5c4926b7ea87f9103ee4ea.s1.eu.hivemq.cloud', 
            port: 8883,
            protocol: 'mqtts',
            username: 'mqtt-broker',
            password: 'Coolbeans$4'
        }*/
        var mqtt_client = mqtt.connect("mqtt://ec2-3-133-83-209.us-east-2.compute.amazonaws.com", {port:1883});
        //var mqtt_client = mqtt.connect(options); //initialize the MQTT client

        /* setup the callbacks */
        mqtt_client.on('connect', function () {
            console.log('Connected to mqtt broker');
        });
        mqtt_client.on('error', function (error) {
            console.log(error);
        });
        
        /* parsing the message and inserting into mongodb 
            Publishing from esp32 to topic
            1 topic: obstacleCoords
            send in this format: obstacle_x,obstacle_y
        */ 
        
        mqtt_client.on('message', function (topic, message) {
            //Called each time a message is received
            // Insert a single document, wait for promise so we can read it back
            var parse_string = message.toString(); 
            var parse_coord = parse_string.split(',');
            var obstacle_x = Number(parse_coord[0]); 
            var obstacle_y = Number(parse_coord[1]); 
            const p = col.insertOne({ x_coord: obstacle_x, y_coord: obstacle_y});
            console.log('Received message:', topic, message.toString());
        });

         /* subscribe to topic 'my/test/topic' */
         mqtt_client.subscribe('obstacleCoords');
        
         /* publish message 'Hello' to topic 'my/test/topic' */
         mqtt_client.publish('obstacleCoords', '69,52');

        // Find one document
        //const myDoc = await col.findOne();
        // Print to the console
        //console.log(myDoc);
        
       } catch (err) {
        console.log(err.stack);
    }
    /*finally {
        await mongodbClient.close();
    }*/
}

main().catch(console.dir);
mongodbClient.close();


        /* Public HiveMQ Broker                                                                                                                                                              
        Broker: broker.hivemq.com
        TCP Port: 1883
        Websocket Port: 8000
        }*/