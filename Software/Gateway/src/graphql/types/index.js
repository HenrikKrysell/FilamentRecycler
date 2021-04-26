const { gql } = require('apollo-server-express');

export default gql`
  type Query { 
    books: [Book] 
    hello: String
  }
  type Book { title: String, author: String }

  type Subscription {
    serialCommunication: SerialMessage
  }

  type SerialMessage {
    message: String
  }
`;
