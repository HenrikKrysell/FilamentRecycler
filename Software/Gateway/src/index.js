const express = require('express');
const { ApolloServer, gql } = require('apollo-server-express');
import { ControllerClient } from './utils';
import { typeDefs, resolvers} from './graphql';
import { createServer } from 'http';

async function startApolloServer() {
  const apolloServer = new ApolloServer({typeDefs, resolvers/*, subscriptions: { path: '/subscriptions' }*/});
  //await server.start();

  const app = express();
  apolloServer.applyMiddleware({ app });

  const httpServer = createServer(app);
  apolloServer.installSubscriptionHandlers(httpServer);
  
  httpServer.listen({ port: process.env.PORT }, () =>{
    console.log(`🚀 Server ready at http://localhost:${process.env.PORT}${apolloServer.graphqlPath}`)
    console.log(`🚀 Subscriptions ready at ws://localhost:${process.env.PORT}${apolloServer.subscriptionsPath}`)
  })
  // await new Promise(resolve => app.listen({ port: process.env.PORT }, resolve));
  // console.log(`🚀 Server ready at http://localhost:${process.env.PORT}${server.graphqlPath}`);
  // console.log(`🚀 Subscriptions ready at ws://localhost:${process.env.PORT}${server.subscriptionsPath}`);  
  return { apolloServer, app };
}

const sendRequest = async () => {
  try {
    console.log('Addr', process.env.CONTROLLER_ADDR);
    console.log('ControllerClient', ControllerClient);
    const result = await ControllerClient({
      query: 'query {hello(name: "pelle") }',
      variables: { name: "henrik" }
    });
    console.log('result:', result);
  } catch (e) {
    console.log(e);
  }
}

setTimeout(sendRequest, 3000);

startApolloServer();