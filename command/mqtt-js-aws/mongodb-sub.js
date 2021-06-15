import mqtt from 'mqtt';
import mongodb from 'mongodb';

const uri = 'mongodb+srv://tsersiew:coolbeans1234@cluster0.gws9z.mongodb.net/myFirstDatabase?retryWrites=true&w=majority';
var mongodbClient = mongodb.MongoClient(uri,{ useUnifiedTopology: true, useNewUrlParser: true }); //initialises the mongoDB client 

const dbName = "mapdb";

/* description of broker 
    Amazon EC2 Linux Instance 
    equivalent of having local gateway running Linux Distribution 
*/

async function main () {
    try {
        //var start= new Date()
        await mongodbClient.connect();
        //var end = new Date()
        //var diff = end-start;
        //console.log(diff);
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
        //var start= new Date()
        var mqtt_client = mqtt.connect("mqtt://ec2-18-223-15-156.us-east-2.compute.amazonaws.com", {port:1883});
        //var mqtt_client = mqtt.connect(options); //initialize the MQTT client
        //var end = new Date()
        //var diff = end-start;
        //console.log(diff);
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
            if (topic=='obstacle') {
                //color,coordx,coordy
                var parse_string = message.toString(); 
                var parse_coord = parse_string.split(',');
                var colour_num = parse_coord[0];
                var obstacle_x = Number(parse_coord[1]); 
                var obstacle_y = Number(parse_coord[2]); 
                /* add in corresponding colour to the numbers */
                if (colour_num=='0') {var obs_colour = 'red';}
                if (colour_num=='1') {var obs_colour = 'green';}
                if (colour_num=='2') {var obs_colour = 'blue';}
                if (colour_num=='3') {var obs_colour = 'magenta';}
                if (colour_num=='4') {var obs_colour = 'yellow';}
                //var start= new Date()
                //const del = col.deleteMany();
                const p = col.insertOne({ x_coord: obstacle_x, y_coord: obstacle_y, colour: obs_colour, category: 'obstacle position'});
                //var end = new Date()
                //var diff = end-start;
                //console.log(diff);
            }

            else if (topic=='liveloc') {
                //coordx, coordy
                var parse_string = message.toString(); 
                var parse_coord = parse_string.split(',');
                var rover_x = Number(parse_coord[0]); 
                var rover_y = Number(parse_coord[1]); 
                
                const del = col.deleteMany({category: 'rover position'});
                const p = col.insertOne({ x_coord: rover_x, y_coord: rover_y, category: 'rover position'});
            }
    
            console.log('Received message:', topic, message.toString());
        });

         /* subscribe to topic 'my/test/topic' */
         mqtt_client.subscribe('obstacle');
         mqtt_client.subscribe('liveloc');
        
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