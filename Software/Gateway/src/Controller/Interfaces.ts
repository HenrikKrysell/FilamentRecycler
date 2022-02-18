export enum FromControllerMessageType {
    CONSOLE_UPDATED = 'console',
    SEND_COMMAND = 'sendCommand',
    STATE_UPDATED = 'state',
    ERROR = 'controller_error',
    CONNECTION_ERROR = 'connection_error',
}

export enum ToControllerMessageType {
    SET_TARGET_RPM = 'setRPM',
    SHUTDOWN = 'shutdown',
    SET_TARGET_TEMPERATURE = 'setTargetTemperature',
}

export interface IControllerMessage<T> {
    topic: string;
    data: T;
}

export interface IControllerState {
    currentRPM: number;
    targetRPM: number;
    rpmTimestamp: number;
    temperature: number;
    state: string;
}

export interface IControllerConsoleMessage {
    message: string;
}

export interface IControllerSendCommandMessage {
    message: string;
}

export interface IControllerStateMessage {
    stateUpdate: Partial<IControllerState>;
}

export interface IControllerSetRPMMessage {
    targetRPM: number;
}

export interface IControllerSetTargetTemperatureMessage {
    targetTemperature: number;
}

export interface IControllerShutdownMessage {}

export interface IControllerError {
    message: string;
}
