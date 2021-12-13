import React, { useRef, useEffect, useState } from 'react';
import { ConsoleClient } from '../../Streaming';

import { LogViewer } from 'react-log-output';
import ScrollToBottom from 'react-scroll-to-bottom';

const Console = () => {
    const [text, setText] = useState('');

    const createNewStreamingClient = (): ConsoleClient => {
        const client = new ConsoleClient();

        var consoleText = text;
        client.onConnection(() => {});
        client.onConsoleMessage((messages) => {
            consoleText = `${consoleText}\r\n${messages.join('\r\n')}`;
            setText(consoleText);
        });
        client.onError((message) => {
            consoleText = `${consoleText}\r\n${message}`;
            setText(consoleText);
        });

        return client;
    };
    const processDebuggingClient = useRef<ConsoleClient | undefined>();

    useEffect(() => {
        processDebuggingClient.current = createNewStreamingClient();
    }, []);

    return (
        <div key={'content'} style={{ overflow: 'hidden', flexGrow: '1', position: 'relative' }}>
            {/* innerContent css class is defined in index.css. This ScrollToBottom component apparently doesn't accept style attribute */}
            <ScrollToBottom checkInterval={17} className="innerContent">
                <LogViewer text={text} />
            </ScrollToBottom>
        </div>
    );
};

export default Console;
