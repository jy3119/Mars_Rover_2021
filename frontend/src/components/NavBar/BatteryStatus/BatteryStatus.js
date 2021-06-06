import React, {useState} from 'react';

/* Brief 
    Energy will send to local computer using UART 
    From local computer, publish to MQTT Broker to be subscribed to from the web browser 
*/

const BatteryStatus = () => {
    const [batteryStrokes, setBatteryStrokes] = useState(Array(5).fill(true));

    const batteryState; //Track the batteryState 

    /*MQTT Subscribing*/
  var client = mqtt.connect("ws://ec2-3-21-102-39.us-east-2.compute.amazonaws.com/mqtt", {port: 8080, keepalive: 60, clean: true});
  
  //setup the callbacks
  client.on('connect', function () {
    console.log('battery connected to broker');
  });

  client.on('error', function (error) {
    console.log(error);
  });

  //dataString = String(state_num) + "," + String(V_Bat) + "," + String(current_ref) + "," + String(current_measure); 
  
  client.on('message', function (topic, message) {
    //Called each time a message is received
    var parse_string = message.toString(); 
    var parse_coord = parse_string.split(',');
    var state_num = Number(parse_coord[0]); 
    var V_Bat = Number(parse_coord[1]); 
    var current_ref = Number(parse_coord[2]); 
    var current_measure = Number(parse_coord[3]); 
    if (topic == 'battery') {
      Data({x_coord: x, y_coord: y, colour: obs_colour});
    }
    else if (topic == 'liveloc') {
      setpositionData({x_coord: x, y_coord: y});
    }
    console.log('Received message:', topic, message.toString());
});

  // subscribe to topic 'my/test/topic'
  client.subscribe('obstacle');

  client.subscribe('liveloc');

    return (
            <div className="row justify-content-center bg-light">
                <div className="col-lg-12 text-center">
                    <h3>BATTERY STATUS</h3>
                    <p>
                        You can find your battery charge percentage and charging
                        animation which is purely built on reactjs
                    </p>
                </div>
                <div
                    className={`col mb-3 mt-5 col-12 d-flex justify-content-center ${batteryStatusClass}`}
                >
                    <div className="d-flex flex-column battery-outer">
                        {batteryStrokes.map((stroke, idx) => (
                            <BatteryStroke key={idx} visible={stroke} />
                        ))}
                        <div className="battery-charging-indicator">
                            <FontAwesomeIcon icon="bolt" />
                        </div>
                    </div>
                </div>
                <div className="col-12 text-center">
                    <h4>Current Battery: {batteryPercent}%</h4>
                    <div className="col d-inline">
                        {batteryPercent <= 20 && (
                            <div className="d-inline-block mt-3 alert alert-danger">
                                Your battery is getting low, save your work and{" "}
                                <strong>connect your charger</strong> now
                            </div>
                        )}
                        {batteryPercent > 20 && (
                            <div className="d-inline-block mt-3 alert alert-success">
                                Your battery is in{" "}
                                <strong>good condition</strong>
                            </div>
                        )}
                    </div>
                </div>
            </div>
    );
}

export default BatteryStatus;