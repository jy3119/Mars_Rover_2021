var mqtt = require('mqtt')

/*var options = {
    host: '076cff12ed5c4926b7ea87f9103ee4ea.s1.eu.hivemq.cloud',
    port: 8080,
    protocol: 'mqtts',
    username: 'mqtt-broker',
    password: 'Coolbeans$4'
}*/

//initialize the MQTT client
//var client = mqtt.connect(options);
var client = mqtt.connect("ws://ec2-3-21-76-51.us-east-2.compute.amazonaws.com/mqtt", {port: 8080});

//setup the callbacks
client.on('connect', function () {
    console.log('Connected');
});

client.on('error', function (error) {
    console.log(error);
});

client.on('message', function (topic, message) {
    //Called each time a message is received
    console.log('Received message:', topic, message.toString());
});

// subscribe to topic 'my/test/topic'
client.subscribe('instructionsMode');

// publish message 'Hello' to topic 'my/test/topic'
//client.publish('my/test/topic', 'Hello from tser');