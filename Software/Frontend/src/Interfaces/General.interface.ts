import { IMessage } from 'fr-types';

export interface IStreamingClient {
    destroy(): void;
    send(message: IMessage<unknown>): void;
    onConnection(callback: () => void): void;
    isConnected(): boolean;
    onSocketError(handler: (message: string) => void): void;
}
