
import { PubSub, withFilter } from 'graphql-subscriptions';

export const subscriptionTopics = {
  SERIAL_COM: 'SERIAL_COM',
}
export const pubsub = new PubSub();

export default {
  serialCommunication: {
    subscribe: () => pubsub.asyncIterator([subscriptionTopics.SERIAL_COM]),
  },
}