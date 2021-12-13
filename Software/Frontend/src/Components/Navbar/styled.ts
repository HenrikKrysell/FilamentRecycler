import styled from 'styled-components';

interface ButtonProps {
    active: boolean;
}

export const Wrapper = styled.div`
    width: 100%;
    background-color: white;
    color: #696969;
    border-bottom: 1px solid #778899;
    display: flex;
    flex-direction: column;
    z-index: 130;
`;

const NavbarButton = styled.button<ButtonProps>`
    color: #1d3557ff;
    font-size: 20px;
    padding: 10px 60px;
    border-radius: 6px;
    border: 0px;
    margin: 10px 10px;
    cursor: pointer;
    &:disabled {
        color: grey;
        opacity: 0.7;
        cursor: default;
    }
`;

export const NavbarToggleButton = styled(NavbarButton)`
    ${(props) =>
        props.active &&
        `
    background-color: #90aa86ff;
  `}
`;

export const NavbarButtonGroup = styled.div`
    display: flex;
`;
