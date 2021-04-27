import gql from 'graphql-tag';

export const serialCommunicationSubscription = gql`
subscription {
  serialCommunication {
    message
  }
}
`;