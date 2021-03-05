const express = require('express');
const bodyParser = require('body-parser');
const { graphqlExpress, graphiqlExpress } = require('apollo-server-express');
const { makeExecutableSchema } = require('graphql-tools');
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

// The GraphQL schema in string form
const typeDefs = `
  type Query { books: [Book] }
  type Book { title: String, author: String }
`;

// The resolvers
const resolvers = {
  Query: { books: () => books },
};

// Put together a schema
const schema = makeExecutableSchema({
  typeDefs,
  resolvers,
});

//////////////////////////////////////////////////////////

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

//////////////////////////////////////////////////////////

// Initialize the app
const app = express();

// The GraphQL endpoint
app.use('/graphql', bodyParser.json(), graphqlExpress({ schema }));

// GraphiQL, a visual editor for queries
app.use('/graphiql', graphiqlExpress({ endpointURL: '/graphql' }));

// Start the server
app.listen(3000, () => {
  console.log('Go to http://localhost:3000/graphiql to run queries!');
});

