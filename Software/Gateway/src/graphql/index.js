import typeDefs from './types';
import Query from './queries';
import Mutation from './mutations';
import Subscription from './subscriptions';


const resolvers = { Query, /*Mutation,*/ Subscription };
export  { typeDefs, resolvers };