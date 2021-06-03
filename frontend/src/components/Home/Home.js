import React from 'react';
import ChartsEmbedSDK from '@mongodb-js/charts-embed-dom'; 

import RoverMap from './RoverMap/RoverMap'; 

/* Brief 
    Fetch coordinates from mongodb

*/

const Home = () => {
    return (
        <div>
            Map
            Warnings
            <RoverMap /> 
        </div>
    )
}

export default Home; 


