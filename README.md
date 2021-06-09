# Design-Project-2-2021-Summer

The command students will have different solutions that could be developed to communicate with the rover:

-A command-line interface that connects to a remote server over TCP to provide real-time logging and control of the system. This could be uploading commands to a database, that the rover then retrieves and executes.

-A purely local GUI using something like SDL or a static web page,that connects out over MQTT/HTTP/TCP to server.

-A server side interface generated using HTML and something like react.

-A mobile app connecting back into a server with buttons for forward,back, rotate, etc.

-A web application using nodejs and mongodb as backend and reactjs as frontend.

To run the application: 
- in frontend directory, run: 
    ### `npm start`
    Open [http://localhost:5000](http://localhost:5000) to view it in the browser.
- in backend directory, run: 
    ### `npm start`
    Open [http://localhost:3000](http://localhost:3000) to view it in the browser.
- in mqtt-js-aws directory, run mongodb-sub.js file and proxy-battpub.js file with command:
    ### `node mongodb-sub.js` 
    ### `node proxy-battpub.js` 
- Amazon EC2 Instance Public DNS address must be updated everytime instance is stopped. 

mqtt.connect("ws://{Public DNS Address}/mqtt")

Files utilising the address are as listed: 
1. mqtt-js-aws/mongodb-sub.js
2. frontend/src/components/SetCoordinates/DestinationInput/DestinationInput.js
3. frontend/src/components/SetDirections/DirectionButton/DirectionButton.js
4. frontend/src/components/SetDirections/Warning/Warning.js
5. frontend/src/components/NavBar/NavBar.js
6. mqtt-js-aws/proxy-battpub.js
