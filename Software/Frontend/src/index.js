import './index.css';
import React from 'react';
import ReactDOM from 'react-dom';
import {
  ApolloProvider,
} from '@apollo/client';
import * as serviceWorker from './serviceWorker';
import client from './apollo.client';
import MicrocontrollerConsole from './components/MicrocontrollerConsole';

ReactDOM.render(
  // eslint-disable-next-line no-undef
  <ApolloProvider client={client}>
    <React.StrictMode>
      <div>
        <MicrocontrollerConsole />
      </div>
    </React.StrictMode>
  </ApolloProvider>,
  document.getElementById('root'),
);

// If you want your app to work offline and load faster, you can change
// unregister() to register() below. Note this comes with some pitfalls.
// Learn more about service workers: https://bit.ly/CRA-PWA

serviceWorker.unregister();
