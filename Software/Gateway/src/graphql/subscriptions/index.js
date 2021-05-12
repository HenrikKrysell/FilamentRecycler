
import { PubSub, withFilter } from 'graphql-subscriptions';

export const subscriptionTopics = {
  MICROCONTROLLER_COM: 'MICROCONTROLLER_COM',
  CONTROLLER_COM: 'CONTROLLER_COM',
}
export const pubsub = new PubSub();

export default {
  microcontrollerCommunication: {
    subscribe: () => pubsub.asyncIterator([subscriptionTopics.MICROCONTROLLER_COM]),
  },
  controllerCommunication: {
    subscribe: () => pubsub.asyncIterator([subscriptionTopics.CONTROLLER_COM]),
  }
}