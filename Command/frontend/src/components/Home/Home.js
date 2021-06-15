import React from 'react';
import ChartsEmbedSDK from '@mongodb-js/charts-embed-dom'; 
import { Container, Button, TextField, MenuItem } from '@material-ui/core';
import FilterListIcon from '@material-ui/icons/FilterList';
import RoverPos from './RoverPos/RoverPos';

/* Brief 
    Fetch coordinates from mongodb
*/

const sdk = new ChartsEmbedSDK({
    baseUrl: 'https://charts.mongodb.com/charts-design-project-2020-2021-zvubz'
});
const chart = sdk.createChart({ 
    chartId: '181d5c7d-6f42-4774-9715-6287c9a82845',
    height: "500px", 
    autoRefresh: true,
    maxDataAge: 10,
    theme: "dark", 
    showAttribution: false,
}); 

async function renderChart() {
    await chart.render(document.getElementById('chart'));
};

const dist_chart = sdk.createChart({ 
    chartId: '4f8171fa-fd43-4949-b174-bfe231487ec0',
    height: "500px", 
    autoRefresh: true,
    maxDataAge: 10,
    theme: "dark", 
    showAttribution: false,
}); 

async function renderDistChart() {
    await dist_chart.render(document.getElementById('dist_chart'));
};

const obstacle_colours = [
    {
        value: 'All',
        label: 'All',
    },
    {
      value: 'red',
      label: 'red',
    },
    {
      value: 'green',
      label: 'green',
    },
    {
        value: 'blue',
        label: 'blue',
    },
    {
        value: 'pink',
        label: 'pink',
    },
    {
        value: 'yellow',
        label: 'yellow',
    },
  ];

class Home extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
          colour: 'All',
          x_coord: 0, 
          y_coord: 0, 
          angle: 0
        };
        this.handleChange = this.handleChange.bind(this);
        this.handleSubmit = this.handleSubmit.bind(this);
      }
    
    componentDidMount() {
        renderChart();
        renderDistChart();
    }
    componentDidUpdate() {
        chart.setAutoRefresh(10);
        dist_chart.setAutoRefresh(10);
    }

    filter_obstacles () {
        chart.setFilter({category: "obstacle position"})
    }

    filter_rover () {
        chart.setFilter({category: "rover position"})
    }

    handleChange (e) {
        this.setState({colour: e.target.value});
    }

    handleSubmit () {
        chart.setFilter({colour: this.state.colour});
        console.log(this.state.colour);
    }

    render() {
        return(
            <Container maxWidth='lg'>
                    <Container id='chart'>
                    </Container>
                <Container style={{borderRadius: 30,
                            margin: '15px 0',
                            display: 'flex',
                            flexDirection: 'row',
                            justifyContent: 'flex-start',
                            alignItems: 'center',
                            }}>
                    <Button style={{margin: 20}} variant='outlined' color='secondary' onClick={this.filter_obstacles}>Only Obstacles</Button>
                    <Button style={{margin: 20}} variant='outlined' color='secondary' onClick={this.filter_rover}>Only Rover Position</Button>
                    <form autoComplete="off" noValidate onSubmit={this.handleSubmit} style={{display: 'flex', flexWrap: 'wrap', justifyContent: 'center'}}>
                    <TextField 
                        id="obstacle colours"
                        select
                        label="obstacle colours"
                        value={this.state.colour}
                        onChange={this.handleChange}
                        variant="outlined"
                        style={{display: 'flex', width: 200, margin: 20}}
                        color='secondary'
                    >
                    {obstacle_colours.map((option) => (
                    <MenuItem key={option.value} value={option.value}>
                    {option.label}
                    </MenuItem>
                    ))}
                    </TextField>
                    <Button variant="outlined" color="secondary" size="small" style={{display: 'flex', width: 100, height: 75}} type="submit">
                        Filter
                        <FilterListIcon variant="contained" color="secondary">Filter</FilterListIcon>
                    </Button>
                    </form>
                </Container>
                <div style={{display: 'flex', margin: 15, justifyContent: 'center'}}>
                    <RoverPos />
                </div>
                <Container id='dist_chart'>
                </Container>
            </Container>
        )
    }
} 




export default Home; 


