import { EventEmitter } from 'events';
import { ControllerMessageType, IControllerConsoleMessage } from './Interfaces';

class ConsoleLog {
    private controllerEventEmitter: EventEmitter;
    private log: Array<string>;
    private logLimit: number;
    private consoleLogEmitter: EventEmitter;

    constructor(controllerEventEmitter: EventEmitter, logLimit: number) {
        this.log = [];
        this.consoleLogEmitter = new EventEmitter();
        this.logLimit = logLimit;
        this.controllerEventEmitter = controllerEventEmitter;
        this.controllerEventEmitter.on(
            ControllerMessageType.CONSOLE_UPDATED,
            (message: IControllerConsoleMessage) => {
                if (this.log.length >= logLimit) {
                    this.log.shift();
                }
                this.log.push(message.message);
                this.consoleLogEmitter.emit(ControllerMessageType.CONSOLE_UPDATED, message);
            }
        );
    }

    public getLog(): Array<string> {
        return this.log;
    }

    public clear(): void {
        this.log = [];
    }

    public getEventEmitter(): EventEmitter {
        return this.consoleLogEmitter;
    }
}

export default ConsoleLog;
