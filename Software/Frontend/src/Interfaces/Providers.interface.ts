import { IStreamingClient } from './General.interface';

export interface IStreamContext {
    stream: IStreamingClient | undefined;
}
