import React, { useEffect, useRef } from 'react';
import PropTypes from 'prop-types';
import { useLazyQuery, useSubscription } from '@apollo/client';
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
import { serialCommunicationSubscription } from '../Subscriptions';


function SerialConsole() {
  // const { selectedProcessNode, updateCycleTimes } = useStoreState(StructureStore, (s) => ({
  //   selectedProcessNode: s.selectedProcessNode,
  //   updateCycleTimes: s.updateCycleTimes,
  // }));

  // const { selectedFrom, selectedTo, updateGraphsOnReferenceValueSetManually } = useStoreState(UIStore, (s) => ({
  //   selectedFrom: s.from,
  //   selectedTo: s.to,
  //   updateGraphsOnReferenceValueSetManually: s.updateGraphsOnReferenceValueSetManually,
  // }));

  const { data: serialCommunicationSubscriptionData, error, loading } = useSubscription(serialCommunicationSubscription, { variables: {} });

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

  useEffect(() => {
    console.log('serialCommunicationSubscriptionData', serialCommunicationSubscriptionData);
  }, [serialCommunicationSubscriptionData]);

  useEffect(() => {
    console.log('error', error);
  }, [error]);

  const messagesEndRef = useRef(null);

  const scrollToBottom = () => {
    //messagesEndRef.current?.scrollIntoView({ behavior: "smooth" });
  }

  // useEffect(() => {
  //   scrollToBottom();
  // }, [messages]);

  return (
    <Container>
      <HeaderContainer>
        <Header>{'Serial Console'}</Header>
      </HeaderContainer>
      <hr />

      <div>
        {/* {messages.map(message => <Message key={message.id} {...message} />)} */}
        <div ref={messagesEndRef} />
      </div>
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

SerialConsole.propTypes = {
};

SerialConsole.defaultProps = {
};

export default SerialConsole;
