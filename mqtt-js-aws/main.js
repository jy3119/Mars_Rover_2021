var mqtt = require('mqtt')

/*var options = {
    host: '076cff12ed5c4926b7ea87f9103ee4ea.s1.eu.hivemq.cloud',
    port: 8080,
    protocol: 'mqtts',
    username: 'mqtt-broker',
    password: 'Coolbeans$4'
    //added following
    keepalive: 60,
    reconnectPeriod: 1000,
    clean: true,
    encoding: "utf8",
    timeout: 3,
    useSSL: true
}*/

//initialize the MQTT client
//var client = mqtt.connect(options);
var client = mqtt.connect("ws://ec2-3-131-135-32.us-east-2.compute.amazonaws.com/mqtt", {port: 8080});

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
client.subscribe('obstacle');

// publish message 'Hello' to topic 'my/test/topic'
client.publish('obstacle', '12,45,0');
client.publish('obstacle', '9,23,2');


