/*
 *	Brief: This is a proxy publisher to test the MQTT broker (to be during development)
 *		   	Publishes to provided topics which gives messages which will be caught by the subscriber
 *
 *  To enable this proxy publisher:
 * 			1. Open package.json
 * 			2. Look under "scripts"
 * 			3. See the "mqtt" script
 * 			4. Add "\"node dist/server-mqtt/pub\""
 *
 */

/* MQTT publisher */
import mqtt from 'mqtt'

var client = mqtt.connect("ws://18.188.43.23:9001", {
  //open connection with your broker in AWS via websocket
  username:"mqtt-broker", //authenticate your broker with username and password
  password:"coolbeans$4",
}); 


/* test topic */
var testTopic = 'test/'

var test_data = {
	x_coordinate: 6,
	y_coordinate: 87,
}


client.on('connect', () => {
	console.log('Publisher connected!');
    
    var options = {qos: 1}; 

	setInterval(() => {
		client.publish(testTopic, JSON.stringify(test_data), options)
	}, 300000)

	client.on('error', (error) => {
		console.log(error)
	})
})