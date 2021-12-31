import { Socket } from 'net';
import { EventEmitter } from 'events';
import {
    ControllerMessageType,
    IControllerConsoleMessage,
    IControllerError,
    IControllerMessage,
    IControllerSetRPMMessage,
    IControllerState,
    IControllerStateMessage,
} from '.';

export class ControllerClient {
    private eventEmitter: EventEmitter;
    private client: Socket;
    private controllerPort: number;
    private isConnected: boolean;
    private buffer: string;

    constructor(controllerPort: number, eventEmitter: EventEmitter) {
        this.controllerPort = controllerPort;
        this.eventEmitter = eventEmitter;
        this.client = new Socket();
        this.buffer = '';
        this.isConnected = false;

        this.eventEmitter.on(ControllerMessageType.SET_RPM, (msg: IControllerSetRPMMessage) => {
            const message = {
                topic: ControllerMessageType.SET_RPM,
                targetRPM: msg.targetRPM,
            };
            this.client.write(JSON.stringify(message));
        });
    }

    private TryParseMessage(str: string) {
        var messageStart = -1;
        var messageEnd = -1;
        var numStartBracers = 0;
        for (let i = 0; i < str.length; i++) {
            if (str[i] === '{') {
                numStartBracers += 1;
                if (messageStart < 0) {
                    messageStart = i;
                }
            }
            if (str[i] === '}') {
                numStartBracers -= 1;
                if (numStartBracers === 0) {
                    messageEnd = i + 1;
                    break;
                }
            }
        }

        return { messageStart, messageEnd };
    }

    public Connect() {
        if (this.client && !this.client.destroyed) {
            this.client.destroy();
        }

        this.client = new Socket();
        this.client.on('data', (data) => {
            //console.log('Received: ' + data);
            this.buffer += data.toString();
            var parsedMessage: IControllerMessage<unknown> | null = null;
            do {
                parsedMessage = null;
                const { messageStart, messageEnd } = this.TryParseMessage(this.buffer);
                if (messageStart >= 0 && messageEnd >= 0) {
                    const msgString = this.buffer.slice(messageStart, messageEnd);
                    this.buffer = this.buffer.slice(messageEnd);

                    parsedMessage = JSON.parse(msgString) as IControllerMessage<unknown>;
                    if (parsedMessage.topic === 'CONTROLLER_STATE_MESSAGE') {
                        const msg = parsedMessage as IControllerMessage<IControllerState>;

                        this.eventEmitter.emit(ControllerMessageType.STATE_UPDATED, msg.data);
                    }
                    if (parsedMessage.topic === 'RECIEVED_MESSAGE_FROM_MICROCONTROLLER') {
                        const msg: IControllerConsoleMessage = {
                            message: parsedMessage.data as string,
                        };
                        this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
                        //console.log(msg.message);
                    }
                }
            } while (parsedMessage);
        });

        this.client.on('error', (error) => {
            this.isConnected = false;
            const msg: IControllerError = {
                message: error.message,
            };
            this.eventEmitter.emit(ControllerMessageType.CONNECTION_ERROR, msg);

            console.error(`Unable to connect to controller software ${msg.message}`);
            this.client.destroy();
        });

        this.client.on('close', () => {
            this.isConnected = false;
            const msg: IControllerConsoleMessage = {
                message: '[Connection closed]',
            };
            this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
            console.log(msg.message);

            console.log('Sheduling reconnect attemppt');
            setTimeout(() => this.Connect(), 5000);
        });

        this.client.connect(this.controllerPort, 'host.docker.internal', () => {
            this.isConnected = true;
            const msg: IControllerConsoleMessage = {
                message: '[Connect to controller]',
            };
            this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
            console.log(msg.message);
        });
    }
}
