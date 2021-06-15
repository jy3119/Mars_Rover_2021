import React from 'react';

import { useMqttState } from 'mqtt-react-hooks';

export default function Status() {

  /*
  * Status list
  * - Offline
  * - Connected
  * - Reconnecting
  * - Closed
  * - Error: printed in console too
  */
  const { connectionStatus } = useMqttState();

  return (
    <h1>{`Status: ${connectionStatus}`}</h1>
  );
}
/*
var client = mqtt.connect("ws://ec2-3-21-76-51.us-east-2.compute.amazonaws.com/mqtt", {port: 8080});

//setup the callbacks
client.on('connect', function () {
  console.log('Connected');
});*/