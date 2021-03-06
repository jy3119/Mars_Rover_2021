import mqtt from 'mqtt';
import fs from 'fs'; 
/*
var options = {
    //host: 'ec2-18-216-115-209.us-east-2.compute.amazonaws.com',
    port: 3033,
    //protocol: 'wss',
    username: 'mqtt-websockets',
    password: 'coolbeans1234',
    //added following
    //keepalive: 60,
    //reconnectPeriod: 1000,
    //clean: true,
    //encoding: "utf8",
    //timeout: 3,
    useSSL: true
} */

var options = {
    port: 1883,
    username: 'mqtt-websockets',
    password: 'coolbeans1234',
    clean: true,
    //useSSL: true,
    //ca: fs.readFileSync('./ca.crt')
    //key: fs.readFileSync('/etc/keys/client.key');
    //cert: fs.readFileSync('/etc/keys/client.crt');
}

//initialize the MQTT client
//var client = mqtt.connect("mqtt://ec2-18-216-115-209.us-east-2.compute.amazonaws.com/mqtt", options);
var client = mqtt.connect("ws://ec2-18-223-15-156.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});

//setup the callbacks
client.on('connect', function () {
    console.log('Connected');
});

client.on('error', function (error) {
    console.log(error);
});
var start;

client.on('message', function (topic, message) {
    //Called each time a message is received
    var end = new Date();
    var diff = end-start;
    console.log(diff);
    console.log('Received message:', topic, message.toString());
});

// subscribe to topic 'my/test/topic'
client.subscribe('obstacle');

// publish message 'Hello' to topic 'my/test/topic'
/*
client.publish('obstacle', '0,12,45');
client.publish('obstacle', '2,9,23');
client.publish('obstacle', '1,4,29');
client.publish('obstacle', '3,45,2');
client.publish('obstacle', '4,32,53');
*/
//client.publish('liveloc', '9,4,80');
client.publish('liveloc', '40,45,90'); 
/*
client.publish ('battery', '30,85,1');
start = new Date();
client.publish ('battery', '100,45,0');
*/
//client.publish ('obstacle', '2,500,500');
//client.publish('liveloc', '58,12');
//client.publish('liveloc', '45,17');
//client.publish('liveloc', '58,34');
//client.publish('liveloc', '66,14');
//client.publish('liveloc', '49,12');
//client.publish('liveloc', '55,27,90');


