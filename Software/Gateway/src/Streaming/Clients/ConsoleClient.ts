import { WebSocket } from 'ws';
import AbstractStreamingClient from './AbstractStreamingClient';
import {
    IStateMessage,
    IMessage,
    ISetRPMMessage,
    IErrorMessage,
    IAckMessage,
    ISendCommandMessage,
    FROM_BACKEND_MESSAGE_TYPE,
    IConsoleMessage,
} from 'fr-types';
import { IStreamClient, IStreamingClientProperties } from '../../Interfaces/Streaming.interface';
import { EventEmitter } from 'events';
import { ConsoleLog, FromControllerMessageType, IControllerConsoleMessage } from '../../Controller';

class ConsoleClient extends AbstractStreamingClient implements IStreamClient {
    private controllerEventEmitter: EventEmitter;
    private consoleLog: ConsoleLog;

    constructor(
        socket: WebSocket,
        properties: IStreamingClientProperties,
        controllerEventEmitter: EventEmitter,
        consoleLog: ConsoleLog
    ) {
        super(socket, properties);

        this.consoleLog = consoleLog;
        this.controllerEventEmitter = controllerEventEmitter;

        this.consoleLog
            .getEventEmitter()
            .on(FromControllerMessageType.CONSOLE_UPDATED, (message: IControllerConsoleMessage) => {
                const data: IConsoleMessage = {
                    messages: [message.message],
                };
                this.send({
                    type: FROM_BACKEND_MESSAGE_TYPE.CONSOLE_UPDATED,
                    data,
                });
            });

        // Send initial log
        const log = this.consoleLog.getLog();
        if (log.length > 0) {
            const data: IConsoleMessage = {
                messages: log,
            };
            this.send({
                type: FROM_BACKEND_MESSAGE_TYPE.CONSOLE_UPDATED,
                data,
            });
        }
    }

    onMessage(message: IMessage<unknown>): void {
        switch (message.type) {
            case FROM_BACKEND_MESSAGE_TYPE.SEND_COMMAND:
                const setTargetRpmMessage = message as IMessage<ISendCommandMessage>;
                this.handleSendCommand(setTargetRpmMessage.data);
                break;
        }
    }

    handleSendCommand(message: ISendCommandMessage): void {
        this.controllerEventEmitter.emit(FromControllerMessageType.SEND_COMMAND, message.message);
        this.send({
            type: FROM_BACKEND_MESSAGE_TYPE.ACK,
            data: {
                success: true,
            },
        });
    }
}

export default ConsoleClient;
