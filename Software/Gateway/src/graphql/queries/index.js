import { pubsub, subscriptionTopics }  from '../subscriptions';
  
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

  export default {
    hello: () => { pubsub.publish(subscriptionTopics.SERIAL_COM, { serialCommunication: { message: "Hello World!"}}); return 'Hello world!'},
    books: () => books,
  };
