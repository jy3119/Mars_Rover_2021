import { makeStyles } from '@material-ui/core/styles';

export default makeStyles((theme) => ({
    battery_outer: {
        height: '210px',
        width: '100px',
        border: '2px solid #222',
        padding: '4px',
        //border-radius: 7px,
    },

    battery_inner: {
        width: '100%',
        margin: '2px 0',
        background: '#01770a',
    }, 

    /*
    .battery-empty .battery-inner {
        background: #8b0000;
    }
    .battery-outer::before {
        content: "";
        width: 24px;
        position: absolute;
        top: -8px;
        height: 8px;
        background: #222;
        left: 49%;
        border-radius: 4px 4px 3px 3px;
    }
    .battery-outer .battery-charging-indicator {
        position: absolute;
        top: 40%;
        left: 49.2%;
        color: #cbd71d;
        font-size: 40px;
        display: none;
    }
    .battery-charging .battery-outer .battery-charging-indicator {
        display: block;
        animation: fadeIn 0.8s infinite alternate;
    }*/
}));