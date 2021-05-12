const { gql } = require('apollo-server-express');

export default gql`
  type Query { 
    books: [Book] 
    hello: String
  }
  type Book { title: String, author: String }

  type Subscription {
    microcontrollerCommunication: MicrocontrollerMessage
    controllerCommunication: ControllerMessage
  }

  type MicrocontrollerMessage {
    message: String
  }

  type ControllerMessage {
    message: String
  }
`;
