/* eslint-disable import/no-mutable-exports */
import {
  ApolloClient, createHttpLink, split, InMemoryCache,
} from '@apollo/client';
import { setContext } from '@apollo/client/link/context';
import { WebSocketLink } from '@apollo/link-ws';
import { getMainDefinition } from '@apollo/client/utilities';

const httpLink = createHttpLink({
  uri: `${window.location.protocol}//${window.location.hostname}${window._env_.GATEWAY_QUERY_ENDPOINT}`,
});

const wsLink = new WebSocketLink({
  uri: `${window._env_.SUBSCRIPTION_PROTOCOL || 'ws'}://${window.location.hostname}${window._env_.GATEWAY_SUBSCRIPTION_ENDPOINT}`,
  options: {
    reconnect: true,
    lazy: true,
  },
});

const link = split(
  ({ query }) => {
    const { kind, operation } = getMainDefinition(query);
    return kind === 'OperationDefinition' && operation === 'subscription';
  },
  wsLink,
  httpLink,
);

let client = {};

const memoryCacheOptions = {
  typePolicies: {
    Shift: {
      fields: {
        days: {
          merge(_ignored, incoming) {
            return incoming;
          },
        },
      },
    },
  },
};

client = new ApolloClient({
  link,
  cache: new InMemoryCache(memoryCacheOptions),
});

export default client;
