import { WebSocket } from 'ws';
import { IMessage, STREAMING_CONTEXT } from 'fr-types';

export interface IStreamingClientProperties {
    context: STREAMING_CONTEXT;
    protocols: Record<string, string>;
}

export interface IStreamingServerOptions {
    port: number;
    contextHandler: (ws: WebSocket, properties: IStreamingClientProperties) => IStreamClient;
}

export interface IStreamClient {
    getId(): string;
    destroy(): void;
    send<T>(message: IMessage<T>): void;
    onMessage(message: IMessage<unknown>): void;
    onDestroyed(callback: () => void): void;
    getProperties(): IStreamingClientProperties;
}
