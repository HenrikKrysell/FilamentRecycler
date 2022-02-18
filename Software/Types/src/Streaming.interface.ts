import { FROM_BACKEND_MESSAGE_TYPE, TO_BACKEND_MESSAGE_TYPE } from ".";

export interface IMessage<T> {
  type: FROM_BACKEND_MESSAGE_TYPE | TO_BACKEND_MESSAGE_TYPE;
  data: T;
}

export interface IStateMessage {
  currentRPM: number;
  targetRPM: number;
  temperature: number;
  state: string;
}

// FROM_BACKEND_MESSAGE_TYPE
export interface IConsoleMessage {
  messages: Array<string>;
}

export interface IErrorMessage {
  message: string;
}

export interface IAckMessage {
  success: boolean;
}

export interface ISendCommandMessage {
  message: string;
}

// TO_BACKEND_MESSAGE_TYPE
export interface ISetRPMMessage {
  targetRPM: number;
}

export interface ISetTargetTemperatureMessage {
  targetTemperature: number;
}

export interface IShutdownMessage {}
