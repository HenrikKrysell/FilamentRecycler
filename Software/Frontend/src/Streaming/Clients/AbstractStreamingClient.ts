import { IStreamingClient } from '../../Interfaces/General.interface';
import { IMessage, STREAMING_CONTEXT } from 'fr-types';

abstract class StreamingClient implements IStreamingClient {
    private socket: WebSocket;
    private socketErrorHandler?: (message: string) => void;
    private socketConnectionHandlers: Array<() => void>;

    constructor(context: STREAMING_CONTEXT, parameters: Array<string> = []) {
        this.socketConnectionHandlers = [];
        const socket = new WebSocket(`ws://${window.location.hostname}:3000`, [
            'context',
            context,
            ...parameters,
        ]);
        socket.onerror = (ev: Event) => {
            console.error('Socket connection error', ev);
            if (this.socketErrorHandler) {
                this.socketErrorHandler('Socket connection failed');
            }
        };

        socket.onmessage = this.messageMiddleWare;

        socket.onclose = () => {
            console.log('Socket on close');
        };

        socket.onopen = () => {
            this.socketConnectionHandlers = this.socketConnectionHandlers.filter((listener) => {
                listener();
                return false;
            });
        };

        this.socket = socket;
    }

    onSocketError(handler: (message: string) => void) {
        this.socketErrorHandler = handler;
    }

    isConnected(): boolean {
        return this.socket.readyState === WebSocket.OPEN;
    }

    onConnection(callback: () => void): void {
        if (this.isConnected()) {
            callback();
        } else {
            this.socketConnectionHandlers.push(callback);
        }
    }

    destroy() {
        this.socket.close();
    }

    private messageMiddleWare = (payload: MessageEvent<string>) => {
        const message: IMessage<unknown> = JSON.parse(payload.data);
        this.onMessage(message);
    };

    send<T>(message: IMessage<T>) {
        this.socket.send(JSON.stringify(message));
    }

    abstract onMessage(message: IMessage<unknown>): void;
}

export default StreamingClient;
