import { GraphQLClient } from 'graphql-request';

export default ({ endpoint, query, variables}) => {
  return new GraphQLClient(endpoint || `${process.env.CONTROLLER_ADDR}`, {
    headers: {}
  })
  .request(query, variables);
}