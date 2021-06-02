var mqtt = require('mqtt');
var mongodb = require('mongodb');

const uri = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var mongodbClient = mongodb.MongoClient(uri,{ useUnifiedTopology: true, useNewUrlParser: true }); //initialises the mongoDB client 

const dbName = "test";

async function main () {
    try {
        await mongodbClient.connect();
        console.log("Connected to mongodb atlas");
        const db = mongodbClient.db(dbName);
        // Use the collection "test"
        const col = db.collection("test");
        /*var options = {
            host: '076cff12ed5c4926b7ea87f9103ee4ea.s1.eu.hivemq.cloud', 
            port: 8883,
            protocol: 'mqtts',
            username: 'mqtt-broker',
            password: 'Coolbeans$4'
        }*/
        var mqtt_client = mqtt.connect("mqtt://ec2-18-218-199-204.us-east-2.compute.amazonaws.com", {port:1883});
        //var mqtt_client = mqtt.connect(options); //initialize the MQTT client
        //setup the callbacks
        mqtt_client.on('connect', function () {
            console.log('Connected to mqtt broker');
        });
        mqtt_client.on('error', function (error) {
            console.log(error);
        });

        mqtt_client.on('message', function (topic, message) {
            //Called each time a message is received
            // Insert a single document, wait for promise so we can read it back
            var parse_string = message.toString(); 
            var parse_coord = parse_string.split(',');
            var first_name_string = parse_coord[0]; 
            var last_name_string = parse_coord[1]; 
            var birth_string = parse_coord[2]; 
            var death_string = parse_coord[3]; 
            var contribs_string = parse_coord[4]; 
            var views_string = parse_coord[5]; 
            const p = col.insertOne({ name: {first: first_name_string, last: last_name_string}, birth: birth_string, death: death_string, contribs: contribs_string, views: views_string});
            console.log('Received message:', topic, message.toString());
        });

         // subscribe to topic 'my/test/topic'
         mqtt_client.subscribe('my/test/topic');
        
         // publish message 'Hello' to topic 'my/test/topic'
         mqtt_client.publish('my/test/topic', 'Alan,Turing,1912-06-22T17:00:00.000+00:00,1954-06-06T16:30:00.000+00:00,Turingmachine,1230000');

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