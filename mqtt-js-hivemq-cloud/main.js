var mqtt = require('mqtt')

var options = {
    host: '076cff12ed5c4926b7ea87f9103ee4ea.s1.eu.hivemq.cloud',
    port: 8883,
    protocol: 'mqtts',
    username: 'mqtt-broker',
    password: 'Coolbeans$4'
}

//initialize the MQTT client
var client = mqtt.connect(options);

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
client.subscribe('my/test/topic');

// publish message 'Hello' to topic 'my/test/topic'
client.publish('my/test/topic', 'Hello from tser');