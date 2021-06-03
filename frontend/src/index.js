import React from 'react';
import ReactDOM from 'react-dom';
import { Provider } from 'react-redux';
import { createStore, applyMiddleware, compose } from 'redux';
import thunk from 'redux-thunk';
import ChartsEmbedSDK from '@mongodb-js/charts-embed-dom'; 

import reducers from './reducers';
import App from './App';
import './index.css'; 

const store = createStore(reducers, compose(applyMiddleware(thunk)));

const sdk = new ChartsEmbedSDK({
  baseUrl: 'https://charts.mongodb.com/charts-design-project-2020-2021-zvubz'
});
const chart = sdk.createChart({ chartId: '181d5c7d-6f42-4774-9715-6287c9a82845' }); 

ReactDOM.render(
  <Provider store={store}>
    <App />
  </Provider>, 
  document.getElementById('root')
);


