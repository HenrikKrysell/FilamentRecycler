const express = require('express');
const { ApolloServer, gql } = require('apollo-server-express');
import { ControllerClient } from './utils';
import { typeDefs, resolvers} from './graphql';
import { createServer } from 'http';

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

// const sendRequest = async () => {
//   try {
//     console.log('Addr', process.env.CONTROLLER_ADDR);
//     console.log('ControllerClient', ControllerClient);
//     const result = await ControllerClient({
//       query: 'query {hello(name: "pelle") }',
//       variables: { name: "henrik" }
//     });
//     console.log('result:', result);
//   } catch (e) {
//     console.log(e);
//   }
// }

// setTimeout(sendRequest, 3000);

/////////////////////////////////
// var buffer = require('buffer');
// var udp = require('dgram');

// // creating a client socket
// var client = udp.createSocket('udp4');

// //buffer msg
// var data = Buffer.from('siddheshrane');

// client.on('message',function(msg,info){
//   console.log('Data received from server : ' + msg.toString());
//   console.log('Received %d bytes from %s:%d\n',msg.length, info.address, info.port);
// });

// //sending msg
// client.send(data,5001,'controller',function(error){
//   if(error){
//     client.close();
//   }else{
//     console.log('Data sent !!!');
//   }
// });

// var data1 = Buffer.from('hello');
// var data2 = Buffer.from('world');

// //sending multiple msg
// client.send([data1,data2],5001,'controller',function(error){
//   if(error){
//     client.close();
//   }else{
//     console.log('Data sent !!!');
//   }
// });
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
	client.destroy(); // kill client after server's response
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