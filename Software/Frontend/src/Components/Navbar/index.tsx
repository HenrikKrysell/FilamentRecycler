import React, { useEffect, useState } from 'react';
import { Wrapper, NavbarToggleButton, NavbarButtonGroup } from './styled';
import { useHistory } from 'react-router';
import { useTranslation } from 'react-i18next';
import styled from 'styled-components';
import { ROUTE } from '../../Types/enums';

const getActiveTab = (pathname: string): ROUTE => {
    if (pathname.startsWith(ROUTE.CURRENT_SESSION)) {
        return ROUTE.CURRENT_SESSION;
    } else if (pathname.startsWith(ROUTE.CONSOLE)) {
        return ROUTE.CONSOLE;
    } else {
        return ROUTE.RECORDED_SESSION;
    }
};

const Navbar = () => {
    const history = useHistory();
    const [active, setActive] = useState(getActiveTab(history.location.pathname));

    const handleRecordingClicked = () => {
        setActive(ROUTE.RECORDED_SESSION);
        history.push(ROUTE.RECORDED_SESSION);
    };

    const handleConsoleClicked = () => {
        setActive(ROUTE.CONSOLE);
        history.push(ROUTE.CONSOLE);
    };

    const handleLiveClicked = () => {
        setActive(ROUTE.CURRENT_SESSION);
        history.push(ROUTE.CURRENT_SESSION);
    };

    useEffect(() => {
        return history.listen((location) => {
            setActive(getActiveTab(location.pathname));
        });
    }, [history]);

    return (
        <Wrapper>
            <NavbarButtonGroup>
                <NavbarToggleButton
                    key={ROUTE.CURRENT_SESSION}
                    active={active === ROUTE.CURRENT_SESSION}
                    onClick={handleLiveClicked}
                >
                    Live session
                </NavbarToggleButton>
                <NavbarToggleButton
                    key={ROUTE.CONSOLE}
                    active={active === ROUTE.CONSOLE}
                    onClick={handleConsoleClicked}
                >
                    Console
                </NavbarToggleButton>
                <NavbarToggleButton
                    key={ROUTE.RECORDED_SESSION}
                    active={active === ROUTE.RECORDED_SESSION}
                    onClick={handleRecordingClicked}
                >
                    Recorded session
                </NavbarToggleButton>
            </NavbarButtonGroup>
        </Wrapper>
    );
};

export default Navbar;
