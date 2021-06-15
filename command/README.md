# Command Subsystem
## A web application using nodejs and mongodb as backend and reactjs as frontend.

The Command subsystem web application can be divided into 3 parts: 
- MQTT Broker: mqtt-js-aws
- Web App frontend: frontend
- Web App backend: backend

To run the application on web: 
- go to site https://design-project-2-2021-summer-try2.netlify.app on Safari browser 
- cannot run secure WebSockets since domain name was not purchased

To run the application on localhost: 
- Before proceeding, make sure that you have Node Js installed on your local machine.
- in frontend directory, run: 
    #### `npm install`
    #### `npm run build`
    #### `npm start`
    Open [http://localhost:5000](http://localhost:5000) to view it in the browser.
- in backend directory, run: 
    #### `npm install`
    #### `npm start`
    Open [http://localhost:3000](http://localhost:3000) to view it in the browser.
- in mqtt-js-aws directory, run mongodb-sub.js file, distance.js and battpub.js file with command:
    #### `node mongodb-sub.js` 
    #### `node battpub.js` 
    #### `node distance.js` 
- Amazon EC2 Instance Public DNS address must be updated everytime instance is stopped. 

mqtt.connect("ws://{Public DNS Address}/mqtt")

Files utilising the address are as listed: 
1. mqtt-js-aws/mongodb-sub.js
2. frontend/src/components/SetCoordinates/DestinationInput/DestinationInput.js
3. frontend/src/components/SetDirections/DirectionButton/DirectionButton.js
4. frontend/src/components/SetDirections/Warning/Warning.js
5. frontend/src/components/NavBar/NavBar.js
6. mqtt-js-aws/battpub.js
7. mqtt-js-aws/distance.js

