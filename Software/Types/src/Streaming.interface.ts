import { MESSAGE_TYPE } from ".";

export interface IMessage<T> {
  type: MESSAGE_TYPE;
  data: T;
}

export interface IStateMessage {
  currentRPM: number;
  targetRPM: number;
  temperature: number;
  state: string;
}

export interface IConsoleMessage {
  messages: Array<string>;
}

export interface ISetRPMMessage {
  targetRPM: number;
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
