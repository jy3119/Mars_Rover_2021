import React, { useEffect, useState } from 'react';
import ReactDOM from 'react-dom';
/*
import {
    BarChart,
    Bar,
    Line,
    LineChart,
    XAxis,
    YAxis,
    Tooltip
  } from 'recharts';
  import * as parkData from "./data/skateboard-parks.json"; 
  */
  import ChartsEmbedSDK from '@mongodb-js/charts-embed-dom'; 

/* Brief 
    Fetch coordinates from mongodb
    Charts Based URL: https://charts.mongodb.com/charts-design-project-2020-2021-zvubz
    Chart ID: 181d5c7d-6f42-4774-9715-6287c9a82845  
*/

const RoverMap = () => {
  return(
    <div>
      map
    </div>
  );
}; 

export default RoverMap;


   


//RoverMap is a client to the socket io 
/*
const socket = io('http://localhost:5000', {
  transports: ['websocket', 'polling']
});


const RoverMap = () => {
    const [obstacleData, setobstacleData] = useState({
        x_coord: 0, y_coord: 0
    }); 

    //listen for a 'obstacle' event and update the state
    useEffect(()=> {
        socket.on('obstacle', (obst_coord)=> {
            setobstacleData((currentData)=>[...currentData, obst_coord]);
            /*
            const marker = new H.map.Marker(obst_coord);
            map.addObject(marker);
            
        })
    },[]);

    //render a chart using the state 
    // or add new marker 
    return (
        <div>
            <h1>Real Time CPU Usage</h1>
            {parkData.features.map(park => (
        <Marker
          key={park.properties.PARK_ID}
          position={{
            lat: park.geometry.coordinates[1],
            lng: park.geometry.coordinates[0]
          }}
          onClick={() => {
            setSelectedPark(park);
          }}
          icon={{
            url: `/skateboarding.svg`,
            scaledSize: new window.google.maps.Size(25, 25)
          }}
        />
      ))}

      {selectedPark && (
        <InfoWindow
          onCloseClick={() => {
            setSelectedPark(null);
          }}
          position={{
            lat: selectedPark.geometry.coordinates[1],
            lng: selectedPark.geometry.coordinates[0]
          }}
        >
          <div>
            <h2>{selectedPark.properties.NAME}</h2>
            <p>{selectedPark.properties.DESCRIPTIO}</p>
          </div>
        </InfoWindow>
            <LineChart width={500} height={300} data={obstacleData}>
                <XAxis dataKey="name" />
                <YAxis />
                <Line dataKey="value" />
            </LineChart>
        </div>
    )
}; */ 
