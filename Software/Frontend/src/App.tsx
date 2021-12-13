import React, { useEffect, useState } from 'react';
import { BrowserRouter, Switch, Route } from 'react-router-dom';
import { Navbar } from './Components';
import { ROUTE } from './Types/enums';
import { CurrentSession, NotFound, Console } from './Views';

const App = () => {
    return (
        <BrowserRouter basename={process.env.PUBLIC_URL}>
            <div style={{ height: '100vh', display: 'flex', flexDirection: 'column' }}>
                <div style={{ textAlign: 'center' }}>Filament Recycler</div>
                <Navbar />
                <Switch>
                    <Route exact path="/">
                        <CurrentSession />
                    </Route>
                    <Route exact path={ROUTE.CURRENT_SESSION}>
                        <CurrentSession />
                    </Route>
                    <Route exact path={ROUTE.CONSOLE}>
                        <Console />
                    </Route>
                    <Route exact path={ROUTE.RECORDED_SESSION}>
                        <NotFound />
                    </Route>
                    <Route path="*">
                        <NotFound />
                    </Route>
                </Switch>
            </div>
        </BrowserRouter>
    );
};
export default App;

// //Color schema
// /* CSS HEX */
// --dutch-white: #dccca3ff;
// --dark-sea-green: #90aa86ff;
// --bittersweet: #f96f5dff;
// --celadon-blue: #457b9dff;
// --prussian-blue: #1d3557ff;

// /* CSS HSL */
// --dutch-white: hsla(43, 45%, 75%, 1);
// --dark-sea-green: hsla(103, 17%, 60%, 1);
// --bittersweet: hsla(7, 93%, 67%, 1);
// --celadon-blue: hsla(203, 39%, 44%, 1);
// --prussian-blue: hsla(215, 50%, 23%, 1);

// /* SCSS HEX */
// $dutch-white: #dccca3ff;
// $dark-sea-green: #90aa86ff;
// $bittersweet: #f96f5dff;
// $celadon-blue: #457b9dff;
// $prussian-blue: #1d3557ff;

// /* SCSS HSL */
// $dutch-white: hsla(43, 45%, 75%, 1);
// $dark-sea-green: hsla(103, 17%, 60%, 1);
// $bittersweet: hsla(7, 93%, 67%, 1);
// $celadon-blue: hsla(203, 39%, 44%, 1);
// $prussian-blue: hsla(215, 50%, 23%, 1);

// /* SCSS RGB */
// $dutch-white: rgba(220, 204, 163, 1);
// $dark-sea-green: rgba(144, 170, 134, 1);
// $bittersweet: rgba(249, 111, 93, 1);
// $celadon-blue: rgba(69, 123, 157, 1);
// $prussian-blue: rgba(29, 53, 87, 1);

// /* SCSS Gradient */
// $gradient-top: linear-gradient(0deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-right: linear-gradient(90deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-bottom: linear-gradient(180deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-left: linear-gradient(270deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-top-right: linear-gradient(45deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-bottom-right: linear-gradient(135deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-top-left: linear-gradient(225deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-bottom-left: linear-gradient(315deg, #dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
// $gradient-radial: radial-gradient(#dccca3ff, #90aa86ff, #f96f5dff, #457b9dff, #1d3557ff);
