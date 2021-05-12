import gql from 'graphql-tag';

export const microcontrollerCommunicationSubscription = gql`
subscription {
  microcontrollerCommunication {
    message
  }
}
`;

export const controllerCommunicationSubscription = gql`
subscription {
  controllerCommunication {
    message
  }
}
`;