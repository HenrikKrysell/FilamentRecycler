const express = require('express');
const { ApolloServer, gql } = require('apollo-server-express');
import { ControllerClient } from './utils';
import { typeDefs, resolvers} from './graphql';
import { createServer } from 'http';
import { pubsub, subscriptionTopics } from './graphql/subscriptions';

async function startApolloServer() {
  const apolloServer = new ApolloServer({typeDefs, resolvers, subscriptions: { path: '/subscriptions' }});
  
  const app = express();
  apolloServer.applyMiddleware({ app });

  const httpServer = createServer(app);
  apolloServer.installSubscriptionHandlers(httpServer);
  
  httpServer.listen({ port: process.env.PORT }, () =>{
    console.log(`🚀 Server ready at http://localhost:${process.env.PORT}${apolloServer.graphqlPath}`)
    console.log(`🚀 Subscriptions ready at ws://localhost:${process.env.PORT}${apolloServer.subscriptionsPath}`)
  })
  return { apolloServer, app };
}

/////////////////////////////////

var net = require('net');

var client = new net.Socket();
client.connect(5001, 'controller', function() {
	console.log('Connected');
  const obj = {
    topic: 'test',
    orvar: 10    
  }
	client.write(JSON.stringify(obj));
});

client.on('data', function(data) {
	console.log('Received: ' + data);
  const obj = JSON.parse(data);
  pubsub.publish(subscriptionTopics.MICROCONTROLLER_COM, { microcontrollerCommunication: { message: JSON.stringify(obj)}});
	//client.destroy(); // kill client after server's response
});

client.on('error', function(error) {
	console.log('ERROR: ' + error);
	client.destroy(); // kill client after server's response
});

client.on('close', function() {
	console.log('Connection closed');
});
/////////////////////////////////
startApolloServer();