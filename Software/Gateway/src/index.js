const express = require('express');
const { ApolloServer, gql } = require('apollo-server-express');
import { ControllerClient } from './utils';

// Some fake data
const books = [
  {
    title: "Harry Potter2 and the Sorcerer's stone",
    author: 'J.K. Rowling',
  },
  {
    title: 'Jurassic Park',
    author: 'Michael Crichton',
  },
];

async function startApolloServer() {
  // Construct a schema, using GraphQL schema language
  const typeDefs = gql`
  type Query { 
    books: [Book] 
    hello: String
  }
  type Book { title: String, author: String }
  `;

  // Provide resolver functions for your schema fields
  const resolvers = {
    Query: {
      hello: () => 'Hello world!',
      books: () => books,
    },
  };

  const server = new ApolloServer({ typeDefs, resolvers });
  await server.start();

  const app = express();
  server.applyMiddleware({ app });

  await new Promise(resolve => app.listen({ port: 3000 }, resolve));
  console.log(`🚀 Server ready at http://localhost:3000${server.graphqlPath}`);
  return { server, app };
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