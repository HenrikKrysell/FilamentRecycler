import {
    IErrorMessage,
    IMessage,
    IStateMessage,
    FROM_BACKEND_MESSAGE_TYPE,
    STREAMING_CONTEXT,
} from 'fr-types';
import { IState, StateManager } from '../../Lib/StateManager';
import AbstractStreamingClient from './AbstractStreamingClient';

export default class CurrentSessionClient extends AbstractStreamingClient {
    private stateManager: StateManager;
    private errorHandler?: (message: string) => void;

    constructor(stateManager: StateManager) {
        super(STREAMING_CONTEXT.CURRENT_SESSION, []);
        this.stateManager = stateManager;
        this.onSocketError((errorMessage: string) => {
            if (this.errorHandler) {
                this.errorHandler(errorMessage);
            }
        });
    }

    onMessage(message: IMessage<unknown>): void {
        if (message.type === FROM_BACKEND_MESSAGE_TYPE.STATE_UPDATED) {
            const getProcessnodesResponse = message as IMessage<IStateMessage>;
            this.stateManager.mutate(message.data as IState);
        } else if (message.type === FROM_BACKEND_MESSAGE_TYPE.ERROR) {
            if (this.errorHandler) {
                const errorMessage = message as IMessage<IErrorMessage>;
                this.errorHandler(errorMessage.data.message);
            }
        } else if (message.type === FROM_BACKEND_MESSAGE_TYPE.ACK) {
        } else {
            throw new Error('Unsupported context message');
        }
    }

    onDebugError(handler: (message: string) => void) {
        this.errorHandler = handler;
    }

    // getTT(processId: string): Promise<Array<IExtendedNode>> {
    //     return new Promise((resolve, reject) => {
    //         this.onConnection(() => {
    //             this.getTTListeners.push({
    //                 success: resolve,
    //                 error: reject,
    //             });
    //             if (this.getTTListeners.length === 1) {
    //                 const message = {
    //                     type: 'TT_TYPE',
    //                     data: {
    //                         processId,
    //                     },
    //                 };
    //                 this.send(message);
    //             }
    //         });
    //     });
    // }
}
