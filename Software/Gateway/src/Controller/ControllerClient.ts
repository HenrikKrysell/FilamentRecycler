import { Socket } from 'net';
import { EventEmitter } from 'events';
import {
    ControllerMessageType,
    IControllerConsoleMessage,
    IControllerError,
    IControllerSetRPMMessage,
} from '.';

export class ControllerClient {
    private eventEmitter: EventEmitter;
    private client: Socket;
    private controllerPort: number;

    constructor(controllerPort: number, eventEmitter: EventEmitter) {
        this.controllerPort = controllerPort;
        this.eventEmitter = eventEmitter;
        this.client = new Socket();
    }

    public Connect() {
        if (this.client && !this.client.destroyed) {
            this.client.destroy();
        }

        this.client = new Socket();
        this.eventEmitter.on(ControllerMessageType.SET_RPM, (msg: IControllerSetRPMMessage) => {
            const message = {
                topic: ControllerMessageType.SET_RPM,
                targetRPM: msg.targetRPM,
            };
            this.client.write(JSON.stringify(message));
        });

        this.client.on('data', (data) => {
            console.log('Received: ' + data);

            const obj = data.toJSON();
            const msg: IControllerConsoleMessage = {
                message: data.toString(),
            };
            this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
            console.log(msg.message);
        });

        this.client.on('error', (error) => {
            const msg: IControllerError = {
                message: error.message,
            };
            this.eventEmitter.emit(ControllerMessageType.CONNECTION_ERROR, msg);

            console.error(`Unable to connect to controller software ${msg.message}`);
            this.client.destroy();
        });

        this.client.on('close', () => {
            const msg: IControllerConsoleMessage = {
                message: '[Connection closed]',
            };
            this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
            console.log(msg.message);

            console.log('Sheduling reconnect attemppt');
            setTimeout(() => this.Connect(), 5000);
        });

        this.client.connect(this.controllerPort, 'host.docker.internal', () => {
            const msg: IControllerConsoleMessage = {
                message: '[Connect to controller]',
            };
            this.eventEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, msg);
            console.log(msg.message);
        });
    }
}
