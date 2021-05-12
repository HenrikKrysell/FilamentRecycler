import gql from 'graphql-tag';

export const BooksQuery = gql`
query books{
  books {
    title,
    author
  }
}
`;