import React from 'react';
import ChartsEmbedSDK from '@mongodb-js/charts-embed-dom'; 
import { Button } from '@material-ui/core';
import useStyles from './styles'; 

import RoverMap from './RoverMap/RoverMap'; 
import { ReactReduxContext } from 'react-redux';

/* Brief 
    Fetch coordinates from mongodb

*/

/*
const Home = () => {
    const sdk = new ChartsEmbedSDK({
        baseUrl: 'https://charts.mongodb.com/charts-design-project-2020-2021-zvubz'
    });
    const chart = sdk.createChart({ chartId: '181d5c7d-6f42-4774-9715-6287c9a82845' }); 

    async function renderChart() {
        await chart.render(RoverMap);
    };

    return (
        <div>
            Map
            Warnings
            <Button variant="contained" color="textSecondary" size="small" onClick={renderChart} fullWidth>Clear</Button>
            <RoverMap />
        </div>
    );
}*/ 

const sdk = new ChartsEmbedSDK({
    baseUrl: 'https://charts.mongodb.com/charts-design-project-2020-2021-zvubz'
});
const chart = sdk.createChart({ 
    chartId: '181d5c7d-6f42-4774-9715-6287c9a82845',
    height: "500px"
}); 

async function renderChart() {
    await chart.render(document.getElementById('chart'));
};

class Home extends React.Component {

    render() {
        return(
            <div id='chart'>
                <Button 
                style={{width: 500, height: 500, margin: 0, border: 1}} 
                variant="contained" color="textSecondary" size="small" onClick={renderChart} fullWidth>Clear</Button>
            </div>
            
        )
    }
} 




export default Home; 


