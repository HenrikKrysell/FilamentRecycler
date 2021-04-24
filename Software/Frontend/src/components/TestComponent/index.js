import React, { useEffect } from 'react';
import PropTypes from 'prop-types';
import { useLazyQuery } from '@apollo/client';
import { useStoreState } from 'pullstate';
//import { UIStore } from '../Stores';
import {
  Container,
  Header,
  HeaderContainer,
  PropertyContainer,
  PropertyHeader,
  PropertyValue,
  PropertyHeaderValueContainer,
  ReferenceContainer,
  ReferenceItem,
} from './styled';
import { BooksQuery } from '../Queries';

function TestComponent() {
  // const { selectedProcessNode, updateCycleTimes } = useStoreState(StructureStore, (s) => ({
  //   selectedProcessNode: s.selectedProcessNode,
  //   updateCycleTimes: s.updateCycleTimes,
  // }));

  // const { selectedFrom, selectedTo, updateGraphsOnReferenceValueSetManually } = useStoreState(UIStore, (s) => ({
  //   selectedFrom: s.from,
  //   selectedTo: s.to,
  //   updateGraphsOnReferenceValueSetManually: s.updateGraphsOnReferenceValueSetManually,
  // }));

  const [fetchBooks, { data }] = useLazyQuery(BooksQuery, { fetchPolicy: 'network-only' });

  useEffect(() => {
      fetchBooks();
      // fetchBooks({
      //   variables: {
      //     processNodeId: selectedProcessNode.id,
      //   },
      // });
  }, [fetchBooks]);

  useEffect(() => {
    console.log('data', data);
}, [data]);

  return (
    <Container>
      <HeaderContainer>
        <Header>{'Test'}</Header>
      </HeaderContainer>
      <hr />

      {data && data.books && data.books.map((b) => (
        <PropertyContainer>
          <PropertyHeaderValueContainer>
            <PropertyHeader>{`Title):`}</PropertyHeader>
            <PropertyValue>{b.title}</PropertyValue>
          </PropertyHeaderValueContainer>

          <PropertyHeaderValueContainer>
            <PropertyHeader>{`Author:`}</PropertyHeader>
            <PropertyValue>{b.author}</PropertyValue>
          </PropertyHeaderValueContainer>
        </PropertyContainer>
      ))}
    </Container>
  );
}

TestComponent.propTypes = {
};

TestComponent.defaultProps = {
};

export default TestComponent;
