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

export default class RecordedSessionClient extends AbstractStreamingClient {
    private stateManager: StateManager;
    private errorHandler?: (message: string) => void;

    constructor(stateManager: StateManager) {
        super(STREAMING_CONTEXT.CONSOLE, []);
        this.stateManager = stateManager;
        this.onSocketError((errorMessage: string) => {
            if (this.errorHandler) {
                this.errorHandler(errorMessage);
            }
        });
    }

    onMessage(message: IMessage<unknown>): void {
        if (message.type === MESSAGE_TYPE.CONSOLE_UPDATED) {
            // console.log('STATE_UPDATED');
            // console.log(message);
            const getProcessnodesResponse = message as IMessage<IConsoleMessage>;
            this.stateManager.mutate(message.data as IState);
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

    onDebugError(handler: (message: string) => void) {
        this.errorHandler = handler;
    }
}
