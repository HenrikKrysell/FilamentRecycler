import { WebSocket, RawData } from 'ws';
import { v4 as uuidv4 } from 'uuid';
import { IStreamClient, IStreamingClientProperties } from '../../Interfaces/Streaming.interface';
import { IMessage, IErrorMessage, FROM_BACKEND_MESSAGE_TYPE } from 'fr-types';

export default abstract class AbstractStreamingClient implements IStreamClient {
    private socket: WebSocket;
    private properties: IStreamingClientProperties;
    private id: string;
    private destructionListener?: () => void;

    constructor(socket: WebSocket, properties: IStreamingClientProperties) {
        this.id = uuidv4();
        this.properties = properties;
        this.socket = socket;
        socket.on('error', this.onDestructionEvent);
        socket.on('close', this.onDestructionEvent);
        socket.on('message', this.messageMiddleWare);
    }

    destroy(): void {
        this.socket.close();
    }

    getProperties(): IStreamingClientProperties {
        return this.properties;
    }

    private onDestructionEvent = (): void => {
        if (this.destructionListener) {
            this.destructionListener();
        }
    };

    onDestroyed(callback: () => void): void {
        this.destructionListener = callback;
    }

    messageMiddleWare = (payload: RawData): void => {
        const message: IMessage<unknown> = JSON.parse(payload.toString());
        this.onMessage(message);
    };

    send = <T>(message: IMessage<T>): void => {
        this.socket.send(JSON.stringify(message));
    };

    abstract onMessage(message: IMessage<unknown>): void;

    public getId(): string {
        return this.id;
    }

    protected sendError(message: string): void {
        this.send<IErrorMessage>({ type: FROM_BACKEND_MESSAGE_TYPE.ERROR, data: { message } });
    }
}
