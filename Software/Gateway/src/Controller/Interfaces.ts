export enum ControllerMessageType {
    CONSOLE_UPDATED = 'console',
    SEND_COMMAND = 'sendCommand',
    STATE_UPDATED = 'state',
    SET_RPM = 'setRPM',
    ERROR = 'controller_error',
    CONNECTION_ERROR = 'connection_error',
}

export interface IControllerState {
    currentRPM: number;
    targetRPM: number;
    rpmTimestamp: number;
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

export interface IControllerError {
    message: string;
}