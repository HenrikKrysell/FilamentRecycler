import styled from 'styled-components';

export const Container = styled.div`
  background-color: #fff;
  padding: 0px 30px 20px;
  height: calc(100% - 22px);
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

export const PropertyContainer = styled.div`
    display: grid;
    grid-template-rows: auto
`;

export const PropertyHeaderValueContainer = styled.div`
    display: grid;
    margin-bottom: 10px;
    grid-template-rows: 1fr 1fr
    grid-template-columns: auto;
    grid-template-areas:
    "header"
    "value";
`;

export const PropertyHeader = styled.div`
    grid-area: header;
    font-weight: bold;
`;

export const PropertyValue = styled.span`
    grid-area: value;
`;

export const ReferenceContainer = styled.div`
    display: inline-flex;
    flex-direction: row;
    justify-content: space-between;
`;

export const ReferenceItem = styled.div`

`;
