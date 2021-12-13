import { GraphQLClient } from 'graphql-request';

interface IStorageApiRequest {
    query: string;
    variables?: Record<string, any>;
}

export default async <T>(options: IStorageApiRequest): Promise<T> => {
    return new GraphQLClient(`${process.env.BACKEND_HASURA_ENDPOINT}/`, {}).request(
        options.query,
        options.variables
    );
};
