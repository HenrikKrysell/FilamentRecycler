import {
    IConsoleMessage,
    IErrorMessage,
    IMessage,
    IStateMessage,
    MESSAGE_TYPE,
    STREAMING_CONTEXT,
} from 'fr-types';
import { IState, StateManager } from '../../Lib/StateManager';
import AbstractStreamingClient from './AbstractStreamingClient';

export default class ConsoleClient extends AbstractStreamingClient {
    private errorHandler?: (message: string) => void;
    private consoleMessageHandler?: (messages: Array<string>) => void;

    constructor() {
        super(STREAMING_CONTEXT.CONSOLE, []);
        this.onSocketError((errorMessage: string) => {
            if (this.errorHandler) {
                this.errorHandler(errorMessage);
            }
        });
    }

    onMessage(message: IMessage<unknown>): void {
        if (message.type === MESSAGE_TYPE.CONSOLE_UPDATED) {
            if (this.consoleMessageHandler) {
                const consoleMessage = message as IMessage<IConsoleMessage>;
                this.consoleMessageHandler(consoleMessage.data.messages);
            }
        } else if (message.type === MESSAGE_TYPE.ERROR) {
            if (this.errorHandler) {
                const errorMessage = message as IMessage<IErrorMessage>;
                this.errorHandler(errorMessage.data.message);
            }
        } else if (message.type === MESSAGE_TYPE.ACK) {
        } else {
            throw new Error('Unsupported context message');
        }
    }

    onError(handler: (message: string) => void) {
        this.errorHandler = handler;
    }

    onConsoleMessage(handler: (messages: Array<string>) => void) {
        this.consoleMessageHandler = handler;
    }
}
