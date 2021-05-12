import React, { useEffect, useRef, useState } from 'react';
import PropTypes from 'prop-types';
import { useLazyQuery, useSubscription } from '@apollo/client';
import { useStoreState } from 'pullstate';
//import { UIStore } from '../Stores';
import {
  Container,
  Header,
  HeaderContainer,
  Footer,
  Content
} from './styled';
import { BooksQuery } from '../../graphql/Queries';
import { controllerCommunicationSubscription } from '../../graphql/Subscriptions';


function ControllerConsole() {
  // const { selectedProcessNode, updateCycleTimes } = useStoreState(StructureStore, (s) => ({
  //   selectedProcessNode: s.selectedProcessNode,
  //   updateCycleTimes: s.updateCycleTimes,
  // }));

  // const { selectedFrom, selectedTo, updateGraphsOnReferenceValueSetManually } = useStoreState(UIStore, (s) => ({
  //   selectedFrom: s.from,
  //   selectedTo: s.to,
  //   updateGraphsOnReferenceValueSetManually: s.updateGraphsOnReferenceValueSetManually,
  // }));

  const [messages, setMessages] = useState([]);
  const [userInputText, setUserInputText] = useState('');
  const { data: controllerCommunicationSubscriptionData, error, loading } = useSubscription(controllerCommunicationSubscription, { variables: {} });

  //const [fetchBooks, { data }] = useLazyQuery(BooksQuery, { fetchPolicy: 'network-only' });
  const [isAtBottom, setIsAtBottom] = useState(true);

  // useEffect(() => {
  //   fetchBooks();
  //   // fetchBooks({
  //   //   variables: {
  //   //     processNodeId: selectedProcessNode.id,
  //   //   },
  //   // });
  // }, [fetchBooks]);

  // useEffect(() => {
  //   console.log('data', data);
  // }, [data]);

  useEffect(() => {
    if (controllerCommunicationSubscriptionData && controllerCommunicationSubscriptionData.controllerCommunication) {
      setMessages([...messages, controllerCommunicationSubscriptionData.controllerCommunication.message]);
    }
  }, [controllerCommunicationSubscriptionData]);

  useEffect(() => {
    console.log('error', error);
  }, [error]);

  const messagesEndRef = useRef(null);

  const scrollToBottom = () => {
    if (messagesEndRef.current) {
      messagesEndRef.current.scrollIntoView({ behavior: "smooth" });
    }
  }

  useEffect(() => {
    if (isAtBottom) {
      scrollToBottom();
    }
  }, [messages]);

  const scrollCheck = event => {
    const bottom = event.target.scrollHeight - event.target.scrollTop === event.target.clientHeight;
    if (bottom) {
      console.log("At The Bottom"); //Add in what you want here
    }
    setIsAtBottom(bottom);
  };

  return (
    <Container>
      <HeaderContainer>
        <Header>{'Controller Console'}</Header>
      </HeaderContainer>

      <Content onScroll={scrollCheck}>
        {messages.map(message => (<div>{message}</div>))}
        <div ref={messagesEndRef} />
      </Content>

      <Footer>
        <input
          type="text"
          value={userInputText}
          onChange={(v) => setUserInputText(v)}
        />
      </Footer>
    </Container>
  );
};

ControllerConsole.propTypes = {
};

ControllerConsole.defaultProps = {
};

export default ControllerConsole;
