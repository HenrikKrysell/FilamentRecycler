import styled from 'styled-components';

export const Messages = styled.div`
    overflow: auto;
    height: 100%;
`;

export const Header = styled.h2`
    color: #1f1f1f;
    font-size: 14px;
    margin-bottom: 12px;
    margin-block-start: 0px;
    margin-block-end: 0px;      
`;

export const HeaderContainer = styled.div`
  background-color: #fff;
  height: 50px;
  margin: 0px;
  display: flex;
  flex-direction: row;
  justify-content: space-between;
  align-items: flex-end;
`;

export const Container = styled.div`
display: flex;
flex-flow: column;
height: 100%;
`;

export const Content = styled.div`
overflow: auto;
height: calc(100vh - 100px);
`;

export const Footer = styled.div`
height: 50px;
`;