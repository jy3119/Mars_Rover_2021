import mqtt from 'mqtt';

//initialize the MQTT client
//var client = mqtt.connect(options);
var client = mqtt.connect("ws://ec2-3-21-102-39.us-east-2.compute.amazonaws.com/mqtt", {port: 8080});

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
client.subscribe('battery');

/* publish timer for charging */ 
//assuming 100 minutes to charge fully to 100% 
/*
var percent = 1; 
//interval set at every 1 minute, 60,000 milisec
setInterval(increase, 60000);
function increase() {
    if (percent < 100) {
      percent += 1;
      var mes = (percent.toString()).concat(',45,1');
      client.publish ('battery', mes);
    }
}
*/ 
var percent = 100; 
//interval set at every 1 minute, 60,000 milisec
setInterval(decrease, 60000);
function decrease() {
    if (percent > 20) {
      var mes = (percent.toString()).concat(',45,0');
      client.publish ('battery', mes);
      percent -= 1;
    }
}



